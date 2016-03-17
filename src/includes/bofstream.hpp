#ifndef BOFSTREAM_HPP
#define BOFSTREAM_HPP
/**
  * @file bofstream.hpp
  * @brief Buffered output class
  * @date 23-10-2013
  * @author Maxime Boissonneault
  */
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>

  /** @class bofstream
  * @brief Buffered output class
  *
  * Reduces disk write operation, to speedup computation on shared HPC clusters
  * with limited write heads.
  *
  */
class bofstream : public std::ofstream {
	public:
		explicit bofstream(const char * filename, std::ios_base::openmode mode = std::ios_base::out) :
			std::ofstream(filename,mode),
			oss(mode & std::ostringstream::app),
			max_buffer_size(50*1024*1024)
			{ }

        ~bofstream() { _flush(); std::ofstream::close(); }
        inline std::ostringstream & buffer() { return oss; }
        inline int buffer_size() { return max_buffer_size; }
        inline void buffer_size(int s) { max_buffer_size = s; }
        inline void close() { _flush(); std::ofstream::close(); }
        inline void precision(std::streamsize s) { oss.precision(s); std::ofstream::precision(s); }
        inline void flags(std::ios_base::fmtflags fmtfl) { oss.flags(fmtfl); std::ofstream::flags(fmtfl); }
        inline void setf(std::ios_base::fmtflags fmtfl) { oss.setf(fmtfl); std::ofstream::setf(fmtfl); }
        inline void unsetf(std::ios_base::fmtflags fmtfl) { oss.unsetf(fmtfl); std::ofstream::unsetf(fmtfl); }

        inline void _flush() {
            *this << oss.str();
            oss.str("");
            std::ofstream::flush();
        }
        inline void flush() {
            if (oss.tellp() >= buffer_size()) {
                _flush();
            }
        }
    bofstream& operator<<(bofstream& (*endl)(bofstream&)) { return endl(*this); }
	private:
		std::ostringstream oss;
		int max_buffer_size;
};

inline bofstream & endl(bofstream & o) {
	o.buffer() << std::endl;
	o.flush();
	return o;
}

template <typename T>
inline bofstream & operator<<(bofstream & o, const T & d)
{
	o.buffer() << d;
	return o;
}


#endif //BOFSTREAM_HPP