#ifndef __IGEOOBJ_HPP__
#define __IGEOOBJ_HPP__

#include <core/arithcore.hpp>

// Forward decls
namespace functions 
{
  class IFunction;
  class IVFunction;
}

/// contains various options for the description of geometric object
/*!
** This namespace contains various options for the description of geometric object.
*  It defines the interface IGeoObj which defines a common set of queries. Further various concrete
*  ways for object representions like composition using a set-theoretic approach (CSG) are defined 
*/
namespace objects
{
  /// common interface for all geometric objects
  /*!
  ** This abstract class defines a common interface for all geometric objects.
  *  Every object (represenation) has to support this interface in order for the decomposition
  *  algorithms to work.  
  ** 
  */
  class IGeoObj
  {
  public:
//     inline unsigned primitives() const
//       {
//         return primitives_();
//       }

//     inline unsigned functions() const
//       {
//         return functions_();
//       }

    /*! 
    ** In/Out function of the object
    ** 
    ** cf(i) <= 0 iff i is in the interior of the object
    *  cf(i) >  0 iff i does not lie inside the object 
    **
    ** 
    ** @return characteristic function of the object (in/out)
    */
    inline const functions::IFunction& cf() const
      {
        return cf_();
      }

    /*! 
    ** Dimension of the object
    ** 
    ** 
    ** @return dimension (usually 3)
    */
    inline unsigned dim() const 
      {
	return dim_();
      }

    /*! 
    ** Creates a (deep) copy of the underyling object
    ** Caller gets ownership of the returned copy.
    ** 
    ** @return pointer to the object
    */
    inline IGeoObj* clone() const
      {
	return clone_();
      }

    /** 
     * A function representing the normals of a geometric object
     * The returned function is N(x,y,z) = (cf(x,y,z), N_x, N_y, N_z)
     * 
     * @return nullptr or function representing surface's normals
     */
    inline const functions::IVFunction* normals() const
      {
	return normals_();
      }


    virtual ~IGeoObj()  { }

  private:
    virtual const functions::IFunction& cf_() const = 0;
    virtual IGeoObj* clone_() const = 0;
    virtual unsigned dim_() const = 0;
    virtual const functions::IVFunction* normals_() const = 0;

  private:
    // kein copy ctor und assigment op
    IGeoObj(const IGeoObj &);
    IGeoObj& operator=(const IGeoObj &);

  protected:
    IGeoObj() { }
    
  };

}

#endif /*__IGEOOBJ_HPP__*/

