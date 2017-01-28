/*!
**@file   exception.hpp
**@author Stefan Kiel
**
**@brief  This file defines the exception hierachy of the globopt library
**
**
*/


#ifndef __EXCECPTION_HPP__
#define __EXCECPTION_HPP__ __EXCECPTION_HPP__

#include <boost/exception/info.hpp>
  
/*!
** Throws an exception
**
** @param type type of the exception
** @param msg message
*/
#ifdef _GNUG_
#define THROW(type, msg) { std::cerr << msg << " at " << __PRETTY_FUNCTION__ << std::endl; \
    throw type() << msg_info(msg) << where_info(__PRETTY_FUNCTION__); }
#define S_THROW(msg) throw GlobOptException() << msg_info(msg) << where_info(__PRETTY_FUNCTION__)
#else
#define THROW(type, msg) { std::cerr << msg << " at " << "NOT SUPPROTED" << std::endl; \
    throw type() << msg_info(msg) << where_info("NOT SUPPORTED"); }
#define S_THROW(msg) throw GlobOptException() << msg_info(msg) << where_info("NOT SUPPORTED")
#endif

//typedef boost::error_info<struct tag_errno, int> errno_info;
typedef boost::error_info<struct tag_msg, const char*> msg_info;
typedef boost::error_info<struct tag_where, const char*> where_info;

/// Acts as base class for all thrown exceptions
class GlobOptException : public boost::exception, public std::exception { };

/// Operation not supported exception
/*!
** This exception is thrown if an operation is called which is not
** supported by this class.
**
**
*/
class OpNotSupported : public GlobOptException { };

/// Illegal Arguments exception
/*!
** This exception is thrown if an operation is called with arguments
** which do not satiesfie the preconditions of the operation
**
*/
class IllegalArguments : public GlobOptException { };




#endif
