
#include "nonconvexpolyhedron.inl"
#include <cstdlib>
namespace objects
{
    namespace details
    {
        rvector random_rvector()
        {
            using namespace core::arith;
            using namespace std;
            rvector r(3, 0.0);
            for(int i = 0; i < 3; ++i)
            {
                // Random number in [-1.0, 1.0]
                r[i] = rand() / (RAND_MAX / 2.0) - 1.0;
            }
            //r[0] = 0.0;
            //r[1] = 0.0;
            //r[2] = 1.0;
            //r /= mtl::vector::two_norm(r);
            return r;
        }

        rvector cross(const rvector &first, const rvector &second)
        {
            rvector ret(3, mreal(0.0));
            ret[0] = first[1]*second[2] - first[2]*second[1];
            ret[1] = first[2]*second[0] - first[0]*second[2];
            ret[2] = first[0]*second[1] - first[1]*second[0];
            return ret;
        }

        NonConvexPolyInOut::NonConvexPolyInOut(const Mesh_Types<mreal>::Mesh &mesh)
        {
            // stats:
            unsigned pos = 0, neg = 0;
            for(Mesh::ConstFaceIter cfi = mesh.faces_begin(); cfi != mesh.faces_end(); ++cfi)
            {
                NCPIO_CacheItem ci;
                ci.plane_normal = om_to_mtl_vector( mesh.calc_face_normal(cfi));
                //std::cout << "Plane Normal: " << ci.plane_normal << std::endl;

                // Auslesen aller Eckpunkte und berechnung des "Schwerpunkts"
                ci.centroid = rvector(3, mreal(0.0));
                for(Mesh::ConstFaceVertexIter cfvi = mesh.cfv_iter(cfi); cfvi; ++cfvi)
                {
                    rvector p = om_to_mtl_vector(mesh.point(cfvi));
                    ci.edge_vertices.push_back(p);
                    ci.centroid += p;
                }

                unsigned num_verts = ci.edge_vertices.size();
                //std::cout << ci.centroid;
                ci.centroid /= mreal(num_verts);
                //std::cout << ci.centroid << std::endl;

                // Die Flächennormale
                rvector c1(ci.edge_vertices[1] - ci.edge_vertices[0]);
                rvector c2(ci.edge_vertices[2] - ci.edge_vertices[0]);
                ci.plane_normal = cross(c1, c2);
                ci.plane_normal /= mtl::vector::two_norm(ci.plane_normal);

                // Berechnung der Kanten-Normalen
                for(unsigned i = 0; i < num_verts; ++i)
                {
                    rvector v(ci.edge_vertices[(i + 1) % num_verts]);
                    v -= ci.edge_vertices[i];
                    //rvector n = mtl::vector::cross(ci.plane_normal, v); Existiert nicht!?
                    rvector n = cross(ci.plane_normal, v);
                    // normalisierung nötig?
                    n /= mtl::vector::two_norm(n);
                    rvector pmv(ci.centroid - ci.edge_vertices[i]);
                    mreal dist = mtl::vector::dot(n, pmv);
                    if(dist < 0.0)
                    {
                        ++neg;
                    }
                    else
                    {
                        n *= mreal(-1);
                        ++pos;
                    }
                    ci.edge_normals.push_back(n);
                }

                /*
                ci.inside_face = false;

                for(Mesh::ConstVertexIter cvi = mesh.vertices_begin(); cvi != mesh.vertices_end() && !ci.inside_face; ++cvi)
                {
                    rvector pmv( om_to_mtl_vector(mesh.point(cvi)) - ci.edge_vertices[0] );
                    mreal dist = mtl::vector::dot(pmv, ci.plane_normal);
                    std::cout << dist << std::endl;
                    ci.inside_face |= dist > mreal(1e-5);
                }

                // http://www.softsurfer.com/Archive/algorithm_0101/algorithm_0101.htm#area3D_Polygon%28%29
                // area3D_Polygon( int n, Point* V, Point N )
                ci.surface_area = mreal(0);
                mreal ax, ay, az;  // abs value of normal and its coords
                // select largest abs coordinate to ignore for projection
                ax = abs(ci.plane_normal[0]);     // abs x-coord
                ay = abs(ci.plane_normal[1]);     // abs y-coord
                az = abs(ci.plane_normal[2]);     // abs z-coord

                int coord = 3;                 // coord to ignore: 1=x, 2=y, 3=z
                if (ax > ay) {
                    if (ax > az) coord = 1;    // ignore x-coord
                }
                else if (ay > az) coord = 2;   // ignore y-coord

                unsigned int n = ci.edge_vertices.size();
                // compute area of the 2D projection
                for (unsigned int i=1, j=2, k=0; i<=n; i++, j++, k++)
                {
                    switch (coord)
                    {
                        case 1:
                            ci.surface_area += (ci.edge_vertices[i%n][1] * (ci.edge_vertices[j%n][2] - ci.edge_vertices[k%n][2]));
                            continue;
                        case 2:
                            ci.surface_area += (ci.edge_vertices[i%n][0] * (ci.edge_vertices[j%n][2] - ci.edge_vertices[k%n][2]));
                            continue;
                        case 3:
                            ci.surface_area += (ci.edge_vertices[i%n][0] * (ci.edge_vertices[j%n][1] - ci.edge_vertices[k%n][1]));
                            continue;
                    }
                }

                // scale to get area before projection
                mreal an = sqrt( sqr(ax) + sqr(ay) + sqr(az));  // length of normal vector
                switch (coord)
                {
                    case 1:
                        ci.surface_area *= (an / (2*ax));
                        break;
                    case 2:
                        ci.surface_area *= (an / (2*ay));
                        break;
                    case 3:
                        ci.surface_area *= (an / (2*az));
                }
                ci.surface_area = abs(ci.surface_area);
                */
                m_cache.push_back(ci);
            }

            m_center = rvector(3, mreal(0.0));
            for(Mesh::ConstVertexIter cvi = mesh.vertices_begin(); cvi != mesh.vertices_end(); ++cvi)
            {
                m_center += om_to_mtl_vector(mesh.point(cvi));
                m_vertices.push_back(om_to_mtl_vector(mesh.point(cvi)));
            }


            m_center /= mreal(mesh.n_vertices());


            // Stats:
            int out_faces = 0;
            /*
            std::cout << "Cache-Stats: " << std::endl;
            for(std::vector<NCPIO_CacheItem>::iterator it = m_cache.begin(); it != m_cache.end(); ++it)
            {
                std::cout << "CacheItem mit " << it->edge_normals.size() << " Normalen und " << it->edge_vertices.size() << " Vertizes" << std::endl;
                for(unsigned i = 0; i < it->edge_normals.size(); ++i)
                {
                    std::cout << it->edge_vertices[i] << " ;; " << it->edge_normals[i] << std::endl;
                }
                //std::cout << "Aussenfläche: " << ((it->inside_face)?("nein"):("ja")) << std::endl;;
                //std::cout << "Fläche: " << it->surface_area << std::endl;
                //out_faces += (it->inside_face)?(0):(1);
            }
            std::cout << "Direkte Normalen: " << neg << " Gedrehte Normalen: " << pos << std::endl;
            //std::cout << "Außenflächen: " << out_faces << " (" << static_cast<double>(out_faces) / m_cache.size() * 100.0 << "%)" << std::endl;
            std::cout << "Center: " << m_center << std::endl;
            */
        }

        /*
        bool cachescore_sort(const NCPIO_CacheScored &lhs, const NCPIO_CacheScored &rhs)
        {
            return lhs.second < rhs.second;
        }
        */
    }
}


