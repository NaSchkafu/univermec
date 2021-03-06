#ifndef __MAKE_UNIQUE_HPP__
#define __MAKE_UNIQUE_HPP__

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

#endif /*__MAKE_UNIQUE_HPP__*/
