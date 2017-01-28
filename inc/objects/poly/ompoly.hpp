#ifndef __OMPOLY_HEADER__
#define __OMPOLY_HEADER__

#include "stable.h"

#include "functions/details/fvbase.hpp"

#include "iconvexpoly.hpp"

#include "convexpolyhedron.inl"
#include "nonconvexpolyhedron.inl"

#include "openmesh_types.hpp"


//#include <OpenMesh/Core/IO/MeshIO.hh>

namespace objects
{
    template<typename T>
    class OpenMeshPolyhedron : public IConvexPolyhedron<T>
    {
        typedef typename Mesh_Types<T>::Mesh Mesh;
        private:
            Mesh m_mesh;
            double m_blur;
            functions::IFunction* m_cf;

            virtual const Mesh &mesh_() const
            {
                return m_mesh;
            }

            virtual OpenMeshPolyhedron<T>* clone_() const
            {
                return new OpenMeshPolyhedron<T>(m_mesh);
            }

            virtual OpenMeshPolyhedron<T>* simplify_(const core::arith::ivector &bounding_box) const
            {
                return clone_();
            }

            virtual const functions::IFunction& cf_() const
            {
                return *m_cf;
            }

            virtual unsigned dim_() const
            {
                return 3;
            }

      virtual functions::IVFunction* normals_() const
	{
	  return nullptr;
	}

        public:
            OpenMeshPolyhedron(Mesh mesh)
            {
                using namespace core::arith;
                m_mesh = mesh;

                for(typename Mesh::ConstFaceIter cfi = mesh.faces_begin(); cfi != mesh.faces_end(); ++cfi)
                {
                    std::vector<typename Mesh_Types<T>::Vector> face_verts;

                    for(typename Mesh::ConstFaceVertexIter cfvi = mesh.cfv_iter(cfi); cfvi; ++cfvi)
                    {
                        face_verts.push_back(mesh.point(cfvi));
                    }

                    typename ArithTraits<T>::vec_t n = details::om_to_mtl_vector(mesh.calc_face_normal(cfi));

                    for(typename Mesh::ConstVertexIter cvi = mesh.vertices_begin(); cvi != mesh.vertices_end(); ++cvi)
                    {
                        typename Mesh_Types<T>::Vector p = mesh.point(cvi);
                        if( find(face_verts.begin(), face_verts.end(), p) != face_verts.end() )
                            continue;
                        typename ArithTraits<T>::vec_t pmv( details::om_to_mtl_vector(p - face_verts[0]) );
                        T dist = mtl::vector::dot(pmv, n);
                        //std::cout << dist << std::endl;

                        if( sup(enclosure_cast<interval>(dist)) > 0 )
                        {
                            std::cout << "Nonconvex Poly." << std::endl;
                            m_cf = new functions::details::FVBase< details::NonConvexPolyInOut >(details::NonConvexPolyInOut(mesh));
                            return;
                        }
                    }
                }
                std::cout << "Convex Poly." << std::endl;
                m_cf = new functions::details::FVBase< details::ConvexPolyInOut<T> >(details::ConvexPolyInOut<T>(this));
            }
    };

    namespace io
    {
        OpenMeshPolyhedron<core::arith::mreal>* load_off(std::istream& in);
        OpenMeshPolyhedron<core::arith::mreal>* load_off(const char* filename);

        OpenMeshPolyhedron<core::arith::interval>* load_interval_off(std::istream& in);
        OpenMeshPolyhedron<core::arith::interval>* load_interval_off(const char* filename);

        OpenMeshPolyhedron<core::arith::mreal>* load_obj(const char* filename);
        class FileLoaderException : std::exception {};

        namespace details
        {
          std::string off_vertex(const Mesh_Types<core::arith::mreal>::Vector &v);
          std::string off_vertex(const Mesh_Types<core::arith::interval>::Vector &v);
        }

        template<typename T>
        std::string to_off(const INonConvexPolyhedron<T> &ncp)
        {
          using namespace core::arith;
          typedef typename Mesh_Types<T>::Mesh Mesh;
          std::stringstream ss;
          // "OFF" ist falsch für Interval Polyeder! IE sollte vom Aufrufenden noch davorgesetzt werden.
          ss << "OFF" << std::endl;
          const Mesh &mesh = ncp.mesh();
          ss << mesh.n_vertices() << " " << mesh.n_faces() << " " << mesh.n_edges() << std::endl;
          for(typename Mesh::ConstVertexIter cvi = mesh.vertices_begin(); cvi != mesh.vertices_end(); ++cvi)
          {
            ss << details::off_vertex( mesh.point(cvi) ) << std::endl;
          }
          for(typename Mesh::ConstFaceIter cfi = mesh.faces_begin(); cfi != mesh.faces_end(); ++cfi)
          {
            int i = 0;
            std::stringstream verts;
            for(typename Mesh::ConstFaceVertexIter cfvi = mesh.cfv_iter(cfi); cfvi; ++cfvi)
            {
              i++;
              verts << cfvi.handle() << " ";
            }
            ss << i << " " << verts.str() << std::endl;
          }
          return ss.str();
        }
    }
}

#endif

