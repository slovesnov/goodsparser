/*
 * Vprok.h
 *
 *  Created on: 25.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef VPROK_H_
#define VPROK_H_

#include "GoodsParser.h"

class Vprok: public GoodsParser {
public:
	Vprok();
	void parse0(std::string const& s)override;
	int countPages(std::string const&s)override;
	std::string parseGoods(std::string const& s)override;
//	virtual std::string className()override{
//		return OBJECT_TYPE(*this);
//	}
};

#endif /* VPROK_H_ */
