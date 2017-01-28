#ifndef __nonconvexpolyhedron_inl__
#define __nonconvexpolyhedron_inl__

#include "iconvexpoly.hpp"
#include <boost/numeric/mtl/mtl.hpp>
#include "openmesh_types.hpp"
#include "convexpolyhedron.inl"
#include "decomp/polytree.hpp"

namespace objects
{
    namespace details
    {
        using namespace core::arith;
        using namespace decomp;

        rvector random_rvector(); // templateize this!
        rvector cross(const rvector &first, const rvector &second);

        struct NCPIO_CacheItem // Eine vorkonvertierte Ebene
        {
            rvector centroid;
            rvector plane_normal;
            std::vector<rvector> edge_normals;
            std::vector<rvector> edge_vertices;
            //mreal surface_area;
            //bool inside_face;

            //template<typename T>
            //mreal score(const T &point) const
            //{
            //    T pmv( point - ArithTraits<T>::convert(edge_vertices[0]) );
            //    typename T::value_type distance( mtl::vector::dot(pmv, ArithTraits<T>::force_convert(plane_normal)) );
            //    return abs(ArithTraits<mreal>::force_convert(distance));
            //}
        };

        //typedef std::pair<const NCPIO_CacheItem*, mreal> NCPIO_CacheScored;

        //bool static_score(const NCPIO_CacheItem &lhs, const NCPIO_CacheItem &rhs);
        //bool cachescore_sort(const NCPIO_CacheScored &lhs, const NCPIO_CacheScored &rhs);


        struct NonConvexPolyInOut
        {
            typedef Mesh_Types<mreal>::Mesh Mesh;
            std::vector<NCPIO_CacheItem> m_cache;
            std::vector<rvector> m_vertices;
            rvector m_center;


            // do magic template stuff
            NonConvexPolyInOut(const Mesh_Types<interval>::Mesh)
            {
                assert(!"NonConvexPolyInOut only works with OpenMeshPolyhedron<mreal>");
            }

            NonConvexPolyInOut(const Mesh_Types<aaf>::Mesh)
            {
                assert(!"NonConvexPolyInOut only works with OpenMeshPolyhedron<mreal>");
            }

            NonConvexPolyInOut(const Mesh_Types<taylormodel>::Mesh)
            {
                assert(!"NonConvexPolyInOut only works with OpenMeshPolyhedron<mreal>");
            }

            NonConvexPolyInOut(const Mesh_Types<mreal>::Mesh &mesh);

            ~NonConvexPolyInOut()
            {
            }

            rvector generate_line_vector(int retry) const;

            template<typename T>
            typename T::value_type operator()(const T &arg_point) const
            {
                typedef typename T::value_type value_t;
                // Es werden ersteinmal nur Intervalle verwendet.
                ivector point(enclosure_cast<ivector>(arg_point));
                // Für jedes Face muss der Schnittpunkt berechnet werden
                // Für die Schnittpunktberechnung wird benötigt:
                // Linie: point (Startpunkt), l (Richtungsvektor) (gleich für alle Ebenen!)
                // Ebene: p (Punkt auf der Ebene), n (Normalenvektor)
                bool nice_result = true;
                //int loop = 0;
                int retry = 0;

                //static unsigned vskip = 0;
                //static unsigned stat_retry = 0;

                PolyIVTree::PolyIVTreeNode *tree = PolyIVTree::PolyIVTreeNode::active_node;


#ifdef VERTEX_PRETEST
                if(tree)
                {
                    std::vector<const rvector*> *vertices_inside = tree->get_vertex_list();
                    if(!vertices_inside)
                    {
                        vertices_inside = tree->init_vertex_list(m_vertices);
                    }
                    if( !vertices_inside->empty() )
                    {
                        for(std::vector<const rvector*>::iterator it = vertices_inside->begin(); it != vertices_inside->end(); ++it)
                        {
                            //std::cout << *it << std::endl;
                            if( inf(point[0]) > (**it)[0] || sup(point[0]) < (**it)[0] ||
                                inf(point[1]) > (**it)[1] || sup(point[1]) < (**it)[1] ||
                                inf(point[2]) > (**it)[2] || sup(point[2]) < (**it)[2] )
                            {
                                //std::cout << "Removing: " << *it << std::endl;
                                *it = 0;
                            }

                        }
                        //std::cout << "VL-size: " << vertices_inside->size() << std::endl;
                        vertices_inside->erase(std::remove(vertices_inside->begin(), vertices_inside->end(), static_cast<const rvector*>(0)), vertices_inside->end());
                        //std::cout << "VL-size: " << vertices_inside->size() << std::endl;
                        if(!vertices_inside->empty())
                        {
                            //std::cout << "Vertex Skip. " << vskip++ << "Vertices left: " << vertices_inside->size() << std::endl;
                            return ArithTraits<value_t>::force_convert(interval(-1.0, 1.0));
                        }

                    }
                }
#endif


                unsigned intersections = 0;

                // Lokale Variablen als Ersatz falls kein Baum zur Unterstützung vorhanden ist.
                rvector ray_vect_(3, 0.0);
                std::vector<PolyIVTree::PolyIVTreeNode::face_info_t> gray_planes_;

                const rvector *ray_vect;
                std::vector<PolyIVTree::PolyIVTreeNode::face_info_t>* gray_planes;

                unsigned gray_distances = 0;

                // Erstellen der Liste fürs dynamische Scoring
                //std::vector<NCPIO_CacheScored> scored_list;
                //for(unsigned i = 0; i < m_cache.size(); ++i)
                //    scored_list.push_back( NCPIO_CacheScored( &(m_cache[i]), m_cache[i].score(point) ) );
                //NCPIO_DynamicScore ds(point);
                //std::sort(scored_list.begin(), scored_list.end(), cachescore_sort);

                do
                {
                    gray_distances = 0;
                    intersections = 0;
                    if( tree )
                    {
                        if( retry == 0 )
                        {
                            gray_planes = tree->get_cache();
                            if( !gray_planes )
                                gray_planes = tree->reset_cache(m_cache.size());
                            ray_vect = tree->get_ray_vector();
                        }
                        else
                        {
                            gray_planes = tree->reset_cache(m_cache.size());
                            ray_vect = 0;
                        }
                    }
                    else
                    {
                        gray_planes_ = std::vector<PolyIVTree::PolyIVTreeNode::face_info_t>(m_cache.size(), PolyIVTree::PolyIVTreeNode::NONE);
                        gray_planes = &gray_planes_;
                        ray_vect = 0;
                    }

                    nice_result = true;

                    if( !ray_vect )
                    {
                        if( retry == 0 )
                        {
                            ray_vect_ = forced_cast<rvector>(point);
                            ray_vect_ -= m_center;
                        }
                        //else if( retry < 5 && retry < scored_list.size())
                        //{
                        //    l = scored_list[retry-1].first->plane_normal;
                        //}
                        else
                        {
                            ray_vect_ = random_rvector();
                        }
                        ray_vect = &ray_vect_;
                        if(tree)
                            tree->set_ray_vector(ray_vect);
                    }

                    //const NCPIO_CacheItem &target = *(scored_list[retry % m_cache.size()]);
                    //rvector l = target.plane_normal;
                    //const NCPIO_CacheItem &target = m_cache[retry % m_cache.size()];
                    //rvector l(target.centroid - ArithTraits<rvector>::force_convert(point));
                    //l /= mtl::vector::two_norm(l);

                    //rvector l = random_rvector();

                    for(unsigned i = 0; i < m_cache.size(); ++i)
                    {
                        if( (*gray_planes)[i] == PolyIVTree::PolyIVTreeNode::SKIP )
                            continue;
                        if( (*gray_planes)[i] == PolyIVTree::PolyIVTreeNode::INTERSECTS )
                        {
                            intersections++;
                            continue;
                        }

                        const NCPIO_CacheItem &ci = m_cache[i];
                        const rvector &p = ci.edge_vertices[0];
                        const rvector &n = ci.plane_normal;
                        // Berechnung des Schnittpunkts zwischen Ebene und Line
                        interval num = mtl::vector::dot( ivector(ArithTraits<ivector>::convert(p) - point), ArithTraits<ivector>::convert(n) );
                        mreal denom = mtl::vector::dot(n, *ray_vect);
                        if( abs(denom) < 0.000001 )
                        {
                            // Linie und Ebene sind Parallel. Evtl. auf dieser oder ganz woanders?
                            if( inf(num) <= 0.0 && sup(num) >= 0.0 )
                            {
                                // Linie könnte auf der Ebene liegen.
                                nice_result = false;
                                //std::cout << "Linie evtl. auf der Ebene." << std::endl;
                                //std::cout << num << " " << denom << " " << std::endl << l << std::endl << n << std::endl;
                                break;
                            }
                            else
                            {
                                // Linie ist parallel zur Ebene und nicht auf dieser -> diese Fläche überspringen
                                (*gray_planes)[i] = PolyIVTree::PolyIVTreeNode::SKIP;
                                continue;
                            }
                        }

                        // Ein Schnittpunkt existiert (mit Abstand d zum Linienstartpunkt)
                        interval d = num / ArithTraits<interval>::convert(denom);
                        if(sup(d) < 0.0) // Schnittpunkt ist hinter uns -> skip
                        {
                            // std::cout << "d < 0" << std::endl;
                            (*gray_planes)[i] = PolyIVTree::PolyIVTreeNode::SKIP;
                            continue;
                        }

                        ivector ipl(ArithTraits<ivector>::convert(*ray_vect)); // Intersection Plane Line
                        ipl *= d;
                        ipl += point;

                        interval result(-100);
                        if( (*gray_planes)[i] != PolyIVTree::PolyIVTreeNode::GRAY_DISTANCE )
                        {
                            // In/Out für Schnittpunkt mit konvexem Face
                            for(unsigned j = 0; j < ci.edge_vertices.size(); ++j)
                            {
                                ivector pmv( ipl - ArithTraits<ivector>::convert(ci.edge_vertices[j]) );
                                interval dist( mtl::vector::dot(pmv, ArithTraits<ivector>::convert(ci.edge_normals[j])) );
                                result = max(result, dist);
                            }
                        }


                        if( sup(result) < 0.0 ) // Schnittpunkt ist innerhalb
                        {
                            if( inf(d) < 0.0 ) // Allerdings ist die Distanz nicht sicher -> grau
                            {
                                //std::cout << "Schnittpunkt innerhalb des Faces mit unsicherer Distanz zum Testinterval" << std::endl;
                                if( tree && gray_distances < 3 )
                                {
                                    (*gray_planes)[i] = PolyIVTree::PolyIVTreeNode::GRAY_DISTANCE;
                                    ++gray_distances;
                                }
                                else
                                {
                                    nice_result = false;
                                    break;
                                }

                            }
                            else
                            {
                                ++intersections;
                                (*gray_planes)[i] = PolyIVTree::PolyIVTreeNode::INTERSECTS;
                            }

                        }
                        else if( inf(result) < 0.0) // [ < 0.0; > 0.0] -> Nicht feststellbar. Könnte drinnen oder draussen sein.
                        {
                            //std::cout << "Schnittpunkt nicht sicher innerhalb des Faces." << std::endl;
                            nice_result = false;
                            break;
                        }
                    }
                } while( !nice_result && retry++ < 10);
                //stat_retry += retry;
                //std::cout << "Retries: " << stat_retry << std::endl;
                if( !nice_result )
                {
                    if( tree )
                    {
                        tree->reset_cache( m_cache.size() );
                        tree->set_ray_vector(0);
                    }
                    //std::cout << "." << std::flush;
                    return forced_cast<value_t>(interval(-1.0, 1.0));
                }
                if( gray_distances > 0 )
                {
                    return forced_cast<value_t>(interval(-1.0, 1.0));
                }
                //std::cout << "Echtes Ergebniss: " << intersections << " Schnitte mit dem Polyeder." << std::endl;
                if( intersections % 2 == 0) // gerade Anzahl von Schnitten -> aussen
                {
                    //std::cout << intersections << " -> aussen!" << std::endl;
                    return forced_cast<value_t>(interval(1.0, 1.0));
                }
                else // ungerade Anzahl -> innen.
                {
                    //std::cout << intersections << " -> innen!" << std::endl;
                    return forced_cast<value_t>(interval(-1.0, -1.0));
                }
            }

            std::string text() const
            {
                //ToDo: weitere Informationen?
                return "NonConvex Polyhedron";
            }

            unsigned dim() const
            {
                return 3;
            }
        };
    }

}


#endif
