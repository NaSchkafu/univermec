// -*-c++-*-
#ifndef __convexpolyhedron_hpp__
#define __convexpolyhedron_hpp__

#include "iconvexpoly.hpp"
//#include "../objects/polyhedron.hpp"
#include <boost/numeric/mtl/mtl.hpp>
#include "openmesh_types.hpp"
#include "decomp/polytree.hpp"

namespace objects
{
    namespace details
    {
        using namespace core::arith;



      mreal max(const mreal &a, const mreal &b);
      interval max(const interval &a, const interval &b);
      aaf max(const aaf &a, const aaf &b);
      taylormodel max(const taylormodel& a, const taylormodel &b);

        template<typename P>
        struct ConvexPolyInOut
        {
            //typedef typename P value_type;
            typedef typename ArithTraits<P>::vec_t vec_t;
            typedef typename Mesh_Types<P>::Mesh Mesh;

            const objects::IConvexPolyhedron<P>* m_p;
            std::vector<vec_t> m_vertex_cache;
            std::vector<vec_t> m_normal_cache;

            ConvexPolyInOut(const objects::IConvexPolyhedron<P>* p)
                : m_p(p)
            {
                // Füllen bzw. Berechnen des Caches...
                const Mesh& mesh = m_p->mesh();



                for(typename Mesh::ConstFaceIter fi = mesh.faces_begin(); fi != mesh.faces_end(); ++fi)
                {

                    m_vertex_cache.push_back( om_to_mtl_vector(mesh.point(mesh.cfv_iter(fi))) );
                    /*if( false ) //mesh.has_face_normals() )
                    {
                        // Die Normalen sind vorhanden. Nur auslesen!
                        m_normal_cache.push_back( om_to_mtl_vector(mesh.normal(fi)) );
                    }
                    else
                    {
                        // Normalen sind nicht vorhanden, aber kein mesh.request_face_normals(), da mesh const ist
                        m_normal_cache.push_back( om_to_mtl_vector(mesh.calc_face_normal(fi)) );
                    }
                    */
                    m_normal_cache.push_back( om_to_mtl_vector( Mesh_Types<P>::unormal(mesh, fi)) );
                }

            }

            template<typename T>
            typename T::value_type operator()(const T &point) const
            {
                typedef typename T::value_type value_t;
                assert(m_vertex_cache.size() == m_normal_cache.size());

                decomp::PolyIVTree::PolyIVTreeNode *tree = decomp::PolyIVTree::PolyIVTreeNode::active_node;
                std::vector<decomp::PolyIVTree::PolyIVTreeNode::face_info_t> *cache = 0;
                if( tree )
                {
                    cache = tree->get_cache();
                    if( !cache )
                        cache = tree->reset_cache( m_vertex_cache.size() );
                }

                value_t result = ArithTraits<value_t>::convert(-1.0);
                for(unsigned i = 0; i < m_vertex_cache.size(); ++i)
                {
                    if( cache && (*cache)[i] == decomp::PolyIVTree::PolyIVTreeNode::SKIP )
                        continue;
                    // Normalengleichung: (force convert, falls P nicht in T konvertiert werden kann. Eine Exception wäre hier wohl günstiger.
                    T pmv( point - forced_cast<T>(m_vertex_cache[i]) );
                    value_t dist( mtl::vector::dot(pmv, forced_cast<T>(m_normal_cache[i])) );
                    // Schnitt
                    if ( cache )
                    {
		      if( sup(enclosure_cast<interval>(dist)) < 0 )
                            (*cache)[i] = decomp::PolyIVTree::PolyIVTreeNode::SKIP;
                    }
                    result = max(result, dist);
                }

                return result;
            }

            std::string text() const
            {
                //ToDo: weitere Informationen?
                return "Convex Polyhedron";
            }

            unsigned dim() const
            {
                return 3;
            }
        };
    }

}


#endif
