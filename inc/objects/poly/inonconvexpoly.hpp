#ifndef __INTERFACE_NON_CONVEX_POLYHEDRON__
#define __INTERFACE_NON_CONVEX_POLYHEDRON__

#include "stable.h"
#include "objects/igeoobj.hpp"

#include "openmesh_types.hpp"

namespace objects
{

    template<typename T>
    class IConvexPolyhedron;

    template<typename T>
    class INonConvexPolyhedron : public IGeoObj
    {
        public:
            // Zerlegung in Konvexe Teile (erzeugt neue Objekte)
            inline std::vector< IConvexPolyhedron<T>* > clone_to_convex_parts() const
            {
                return clone_to_convex_parts_();
            }

            inline const typename Mesh_Types<T>::Mesh &mesh() const
            {
                return mesh_();
            }

            inline INonConvexPolyhedron<T>* clone() const
            {
                return clone_();
            }

            inline INonConvexPolyhedron<T>* simplify(const core::arith::ivector &bounding_box) const
            {
                return simplify_(bounding_box);
            }
        private:
            virtual std::vector< IConvexPolyhedron<T>* > clone_to_convex_parts_() const = 0;
            virtual const typename Mesh_Types<T>::Mesh &mesh_() const = 0;
            virtual INonConvexPolyhedron<T>* clone_() const = 0;
            virtual INonConvexPolyhedron<T>* simplify_(const core::arith::ivector &bounding_box) const = 0;
            // kein copy ctor und assigment op
            INonConvexPolyhedron(const INonConvexPolyhedron<T> &);
            INonConvexPolyhedron& operator=(const INonConvexPolyhedron<T> &);

        protected:
            INonConvexPolyhedron() { }
    };
}

#endif
