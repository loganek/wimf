/*
 * idataframe.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef IDATAFRAME_H_
#define IDATAFRAME_H_

#include "../data_types.h"

#include <memory>

namespace Wimf {
namespace DataFrames {

class IDataFrame
{
public:
	virtual ~IDataFrame () {}

	static std::shared_ptr<IDataFrame> parse_frame (const data_array& data);
};

}
}

#endif /* IDATAFRAME_H_ */
