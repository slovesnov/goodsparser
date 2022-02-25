/*
 * Globus.cpp
 *
 *  Created on: 23.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include "Globus.h"
#include <cassert>

Globus::Globus() :
		GoodsParser("globus", "https://online.globus.ru", "?PAGEN_1=") {
	const std::string ti[] =
			{ "catalog-section__item__title", "item-price__rub", "item-price__kop",
					"item-price__additional item-price__additional--solo" };
	for(auto&a:ti){
		m_vtag.push_back(spanClass(a));
	}

}

void Globus::parse0(const std::string &s) {
	TagData t;
	size_t p = 0;
	while ((t = searchSubstring(s, p, "class=\"sub_hover\"><a href=\"",'"' )).found ) {
		m_vpages.push_back({m_stage0url+t.content,0});
		p=t.pos;
	}
}

int Globus::countPages(const std::string &s) {
//	return 1;
	const std::string t = "data-pagecount=\"";
	auto p = s.find(t, 0);
	assert(p!=std::string::npos);
	return atoi(s.c_str() + p + t.length());
}

std::string Globus::parseGoods(const std::string &s) {
	int i;
	std::string s1,s2;
	VTagData vt;
	size_t p = 0;
	while ((vt = getTagsContent(s, p, m_vtag))[0].found) {
		i = 0;
		for (auto &a : vt) {
			assert(a.found);
			s1 = a.content;
			if (i == 0) {
				s1 = trim(s1);//sometimes happens
				s1 = replaceAll(s1, "&nbsp;", " ");
				s1 = replaceAll(s1, "&quot;", "\"");
			} else if (i == 1) {
				//price_rub "1&nbsp;499" -> 1499
				s1 = replaceAll(s1, "&nbsp;", "");
			} else if (i == 3) {
				s1 = trim(s1);
			}
			s2+= m_vtagPrefix[i] +s1;
			i++;
		}
		s2+= "\n";

		p = vt.back().pos;
	}
	return s2;
}
