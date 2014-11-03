/*
 * messageframe.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef MESSAGEFRAME_H_
#define MESSAGEFRAME_H_

#include "idataframe.h"

#include <memory>
#include <string>

namespace Wimf {
namespace DataFrames {

class MessageFrame : public IDataFrame
{
	user_id from, to;
	std::string message;

public:
	virtual ~MessageFrame () {}
	MessageFrame (user_id from, user_id to, const std::string& message);

	static std::shared_ptr<IDataFrame> parse_frame (const data_array& data, data_array::size_type pointer);

	FrameType get_frame_type () override { return FrameType::MESSAGE; }
};

}
}

#endif /* MESSAGEFRAME_H_ */
