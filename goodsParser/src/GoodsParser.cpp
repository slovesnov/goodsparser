/*
 * GoodsParser.cpp
 *
 *  Created on: 23.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include <glib/gstdio.h>
#include <set>

#include "GoodsParser.h"
#include "aslov.h"
#include "help.h"

GoodsParser::GoodsParser(std::string const &stage0url,
		std::string const &pageAdd, std::string const &className) {
	m_stage0url = stage0url;
	m_pageAdd = pageAdd;
	m_className = className;
	std::transform(m_className.begin(), m_className.end(), m_className.begin(), [](unsigned char c) {
		return std::tolower(c);
	});

	g_mkdir(storeFolder.c_str(), 0);
}

void GoodsParser::init(VBool useLocalFiles, bool useManyThreads){
	m_useManyThreads=useManyThreads;
	m_useLocalFiles=useLocalFiles;
	m_threads = useManyThreads ? getNumberOfCores(): 1;
}

void GoodsParser::stage0() {
	auto begin = clock();
	std::string s = getStageFunction(0)(m_stage0url);
	parse0(s);
	println("categories=%lld %s",m_vpages.size(),timeString(begin,true).c_str())
}

DownloadFunctionPtr GoodsParser::getStageFunction(int stage){
	return m_useLocalFiles[stage] ? readLocalFile : downloadFileAndSaveLocally;
}

void GoodsParser::stage1() {
	int i;
	auto begin = clock();
	std::string s;
	m_atom=0;
	m_upper=m_vpages.size();
	m_vthreads.clear();
	for (i = 0; i < m_threads; ++i) {
		m_vthreads.push_back(std::thread(&GoodsParser::th1,this,i));
	}
	for (auto& a : m_vthreads){
		a.join();
	}

//	std::sort(vp.begin(),vp.end(),[](auto& a,auto&b){
//		return a.second<b.second;
//	});

//	std::ofstream o("o.txt");
	for(auto& a:m_vpages){
		s=a.first+m_pageAdd;
		for(i=0;i<a.second;i++){
			m_vgoods.push_back({s+std::to_string(i+1),""});
//			o<<vgoods.back().url<<"\n";
		}
	}

	println("pages=%lld %s",m_vgoods.size(),timeString(begin,true).c_str())
}

void GoodsParser::stage2() {
	int i;
	auto begin = clock();
	m_atom=0;

//	upper=0;
//	for (i = 0; i < 3; ++i) {
//		upper=vp[i].second;
//	}

	m_upper=m_vgoods.size();
	m_vthreads.clear();
	for (i = 0; i < m_threads; ++i) {
		m_vthreads.push_back(std::thread(&GoodsParser::th2,this,i));
	}
	for (auto& a : m_vthreads){
		a.join();
	}

	std::set<std::string> set;
	for(auto& a:m_vgoods){
		for(auto& b:split(a.second,'\n')){
			if(b.length()!=0){
				set.insert(b);
			}
		}
	}

	char b[64];
	time_t rawtime;
	time(&rawtime);
	struct tm *timeinfo = localtime(&rawtime);
	strftime(b, 64, "%d%b%Y.txt", timeinfo);

	std::ofstream o(m_className+b);
	std::ofstream o1("e.txt");
	for(auto& a:set){
		o<<a<<"\n";
		if (g_regex_match_simple("&\\w+;", a.c_str(), GRegexCompileFlags(0),
				GRegexMatchFlags(0))) {
			o1<<a<<"\n";
		}
	}

	mzipFile(m_className+b);

	printl("goods="+std::to_string(set.size())+" totalTime="+secondsToString(begin));
}

void GoodsParser::th1(int t) {
	int i;
	std::string s;
	while ((i = m_atom++) < m_upper ) {
		auto begin=clock();
		auto& v=m_vpages[i];
		v.second = countPages(getStageFunction(1)(v.first));
		s = (m_useManyThreads ? "t" + std::to_string(t) + " " : "")
		 //+v.first+" "
		 + format("category=%2d/%lld pages=%3d", i, m_vpages.size(), v.second) + " "
				+ timeString(begin);
		printzn(s)
		//printl(s)
	}
}

void GoodsParser::th2(int t) {
	int i;
	std::string s;
	while ((i = m_atom++) < m_upper) {
		auto begin = clock();
		auto &a = m_vgoods[i];
		a.second = parseGoods(getStageFunction(2)(a.first));
		s = (m_useManyThreads ? "t" + std::to_string(t) + " " : "")
				+ format("%d/%d goods=%d", i, m_upper,
						countOccurence(a.second, '\n')) + " " + timeString(begin);
		printzn(s)
		//break;
	}
}
