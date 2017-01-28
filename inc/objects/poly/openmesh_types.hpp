#ifndef __OPENMESH_TYPES__
#define __OPENMESH_TYPES__

#include "stable.h"
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

namespace objects
{
    template<typename T>
    struct Mesh_Types
    {
        typedef OpenMesh::VectorT<T,3> Vector;
        struct PolyTraits : public OpenMesh::DefaultTraits
        {
            typedef Vector Point;
            typedef Vector Normal;

            VertexAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status);
            HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge| OpenMesh::Attributes::Status);
            EdgeAttributes(OpenMesh::Attributes::Status);
            FaceAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status);

        };
        typedef OpenMesh::PolyMesh_ArrayKernelT<PolyTraits> Mesh;

        static Vector unormal(const Mesh mesh, const typename Mesh::ConstFaceIter fi)
        {
            typename Mesh::ConstFaceVertexIter fvi = mesh.cfv_iter(fi);
            Vector a = mesh.point(fvi);
            Vector b = mesh.point(++fvi);
            Vector c = mesh.point(++fvi);
            return OpenMesh::cross(b-a, c-a);
        }
    };

    namespace details
    {
        template<typename T>
        typename Mesh_Types<T>::Vector mtl_to_om_vector(const typename core::arith::ArithTraits<T>::vec_t &vect)
        {
            return OpenMesh::VectorT<T, 3>(vect[0], vect[1], vect[2]);
        }

        template<typename T>
        typename core::arith::ArithTraits<T>::vec_t om_to_mtl_vector(const typename OpenMesh::VectorT<T,3> &vect)
        {
            typename core::arith::ArithTraits<T>::vec_t result(3, core::arith::ArithTraits<T>::zero());
            for(int i = 0; i < 3 ; ++i)
                result[i] = vect.values_[i];
            return result;
        }

        template<typename T>
        typename core::arith::ArithTraits<T>::vec_t om_to_mtl_vector(const typename objects::Mesh_Types<T>::Vector &vect)
        {
            typename core::arith::ArithTraits<T>::vec_t result(3, core::arith::ArithTraits<T>::zero());
            for(int i = 0; i < 3 ; ++i)
                result[i] = vect.values_[i];
            return result;
        }
    }
}


#endif
