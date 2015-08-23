/*
 * helloframe.h
 *
 *  Created on: Aug 23, 2015
 *      Author: loganek
 */

#ifndef SRC_SERVER_DATA_FRAMES_HELLOFRAME_H_
#define SRC_SERVER_DATA_FRAMES_HELLOFRAME_H_

#include "idataframe.h"

#include <memory>
#include <string>

namespace Wimf {
namespace DataFrames {

class HelloFrame : public IDataFrame
{
	user_id id;

public:
	virtual ~HelloFrame () {}
	HelloFrame (user_id id);

	static std::shared_ptr<IDataFrame> parse_frame (const DataBuffer& buffer);

	user_id get_id () const { return id; }

	FrameType get_frame_type () override { return FrameType::HELLO; }

	DataBuffer serialize () override;
};

}
}

#endif /* SRC_SERVER_DATA_FRAMES_HELLOFRAME_H_ */
