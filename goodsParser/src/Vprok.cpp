/*
 * Vprok.cpp
 *
 *  Created on: 25.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include "Vprok.h"

Vprok::Vprok() : GoodsParser( "https://www.vprok.ru/catalog","",__func__) {
}

void Vprok::parse0(const std::string &s) {
	TagData t,t1;
	size_t p = 0;
	std::string f;

	while ((t = searchSubstring(s, p, "<a class=\"xf-catalog-categories__link\" href=\"/catalog","\"" )).found ) {
		if(f.empty()){
			f=t.content;
		}
		else{
			if(f==t.content){
				break;
			}
		}
		m_vpages.push_back({m_stage0url+t.content,0});
		p=t.pos;
	}

}

int Vprok::countPages(const std::string &s) {
	const std::string t = "data-owox-total=\"";
	auto p = s.find(t, 0);
	//assert(p!=std::string::npos);
	if(p==std::string::npos){
		return 0;
	}
	int i=atoi(s.c_str() + p + t.length());
	printl(i)
	const int goodsOnPage=30;
	int j=i/goodsOnPage;
	if(i%goodsOnPage!=0){
		j++;
	}
	return j;
}

std::string Vprok::parseGoods(const std::string &s) {
	return "";
}
