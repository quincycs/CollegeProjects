/*
 * File.hpp
 *
 *  Created on: Jun 7, 2010
 *      Author: quincy
 */

#ifndef FILE_HPP_
#define FILE_HPP_

#include <string>
using std::string;
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <boost/crc.hpp>
#include "util/Line_Exception.hpp"

namespace quincySystemLibrary {

class File {
private:
	uint32_t _gcount;
	string filename;
	FILE *file;
public:
	File(string filename);
	/// close the file, if opened.
	virtual ~File();

	/// uses stat to check if the file is visible. (no need to call any open function)
	bool isExisting();

	/// truncates file or creates the file.
	bool create();

	/// Open for reading. The stream is positioned at start of file.
	bool openForOnlyReading();

	/// Open for appending (writing at end of file).  The file  is  created
    /// if it does not exist.  The stream is positioned at EOF.
	bool openForOnlyAppending();
	/// Read from where the stream is positioned.
	/// @param[out] data is where the data will be stored.
	/// @param[in] data_size is how much data will be read.
	/// Returns true iff more than 1 byte is read.
	bool read(void *data, uint32_t data_size);
	uint32_t gcount(){return _gcount;}
	/// Write to where the stream is positioned.
	void write(const void *data, uint32_t data_size);
	/// close the stream.
	void close();

	/// don't use this for files around 1GB which is around 2^32 bytes. 2^10 = KB , 2^20 = MB, 2^30 = GB
	//uint32_t getFileSize();

	/// uses boost library's CRC32 (portable).
	uint32_t getCheckSum();
	/// deletes the file from disk, uses remove() from <stdio.h> (portable)
	bool erase();
	/// unarchives the file using a bash shell to: tar -xzf filename (requires system to have tar)
	bool untar() throw(exception*);
};

}

#endif /* FILE_HPP_ */
