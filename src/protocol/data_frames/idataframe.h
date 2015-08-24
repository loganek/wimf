/*
 * idataframe.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef IDATAFRAME_H_
#define IDATAFRAME_H_

#include "../data_types.h"
#include "../databuffer.h"

#include <memory>

namespace Wimf {
namespace DataFrames {

class IDataFrame
{
public:
	virtual ~IDataFrame () {}

	virtual FrameType get_frame_type () = 0;

	// todo it should be structure, not a tuple
	virtual DataBuffer serialize () = 0;
};

}
}

#endif /* IDATAFRAME_H_ */
