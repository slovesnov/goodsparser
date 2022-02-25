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

const std::string t2 = "data-pagecount=\"";

Globus::Globus() {
	m_stage0url="https://online.globus.ru";
	m_pageAdd="?PAGEN_1=";
	m_className="globus";

	const std::string ti[] =
			{ "catalog-section__item__title", "item-price__rub", "item-price__kop",
					"item-price__additional item-price__additional--solo" };
	for(auto&a:ti){
		tagVI.push_back(spanClass(a));
	}

}

void Globus::parse0(const std::string &s) {
	const std::string ta1 = "class=\"sub_hover\"><a href=\"";
	size_t p = 0,p1;
	while ((p = s.find(ta1, p )) != std::string::npos) {
		p += ta1.length();
		p1 = s.find('"', p);
		assert(p1!=std::string::npos);
		vp.push_back({m_stage0url+s.substr(p, p1 - p),0});
		p=p1+1;
	}
}

int Globus::countPages(const std::string &s) {
//	return 1;
	auto p = s.find(t2, 0);
	assert(p!=std::string::npos);
	return atoi(s.c_str() + p + t2.length());
}

std::string Globus::parseGoods(const std::string &s) {
	int i;
	std::string s1,s2;
	VTagData vt;
	size_t p = 0;
	while ((vt = getTagsContent(s, p, tagVI))[0].found) {
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
			s2+= tagVIPrefix[i] +s1;
			i++;
		}
		s2+= "\n";

		p = vt.back().pos;
	}
	return s2;
}
