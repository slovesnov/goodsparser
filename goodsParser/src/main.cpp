/*
 * main.cpp
 *
 *       Created on: 01.09.2015
 *           Author: aleksey slovesnov
 * Copyright(c/c++): 2015-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         Homepage: slovesnov.users.sourceforge.net
 */

#include "aslov.h"
#include "Globus.h"
#include "Platypus.h"

//useLocalFiles for stages
//const VBool useLocalFiles = {1,1,1};
const VBool useLocalFiles = {0,0,0};
const bool useManyThreads=1;
#define GLOBUS 1

/*
 * globus totalTime=26:20
 */
int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv); //do not remove
	setAllLocales();
	//aslovInit(argv);
	init();

	std::string s, s1;
	VTagData vt;

#if GLOBUS==1
	Globus ob;
#else
	Platypus ob;
#endif

	ob.init(useLocalFiles,useManyThreads);

	println("%s threads=%d useLocalFiles={%s} ",ob.m_className.c_str(),ob.threads,joinV(useLocalFiles).c_str())

	//load categories
	ob.stage0();

	//load pages
	ob.stage1();

	//load goods
	ob.stage2();

	deinit();
}
