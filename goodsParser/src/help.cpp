/*
 * help.cpp
 *
 *  Created on: 21.02.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

/* g_file_new_for_uri & URLDownloadToFile don't work on parameters always return same page, parameter PAGEN_1= is ignored
 * https://online.globus.ru/catalog/rolly-kulinariya-gotovye-blyuda/?PAGEN_1=
 * so use curl library
 */
#include <curl/curl.h>

//zip
#include <mz_compat.h>
#include <zlib.h>

#include "aslov.h"
#include "help.h"

size_t curlWriteFunction(void *ptr, size_t size/*always==1*/, size_t nmemb,
		void *userdata) {
	std::string *p = (std::string*) userdata;
	p->append((const char*) ptr, nmemb);
	return nmemb;
}

std::pair<std::string, bool> downloadFileInner(std::string const &url) {
	CURL *curl = curl_easy_init();
	if (!curl) {
		printel("Failed to init curl")
		return {"",false};
	}
	std::string s, s1;
	auto p = url.find('?');

	//for downloadFileInner("http://localhost/php/jm.php?type12") s=url without split
	if (p == std::string::npos) {
		s = url;
	} else {
		s = url.substr(0, p);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, url.substr(p + 1).c_str());
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");
	}
	curl_easy_setopt(curl, CURLOPT_URL, s.c_str());

	//for downloadFileInner("http://localhost/eggs,ru") works correct
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlWriteFunction);
	/* https://curl.se/libcurl/c/CURLOPT_WRITEDATA.html
	 * curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s); last parameter passed to curlWriteFunction
	 * curlWriteFunction called many times so need to clear s before curl_easy_perform
	 */
	s = "";
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

	//milliseconds if do without timeout program hangs 30'000
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 15'000);
	auto r = curl_easy_perform(curl);
	bool ok = r == CURLE_OK;
	if (!ok) {
//		s1=r==CURLE_OPERATION_TIMEDOUT ? "timeout" : "Failed to get web page code="+std::to_string(int(r));
//		printel(url,s1,"s.len=",s.length());
		s = "";
	}
	curl_easy_cleanup(curl);

	return {s,ok};
}

std::string downloadFile(std::string const &url) {
//	int n=1;
	while (1) {
		auto a = downloadFileInner(url);
		if (a.second) {
			return a.first;
		}
//		printel("attempt",++n)
	}
}

std::string downloadFileAndSaveLocally(std::string const& url){
	auto s=downloadFile(url);
	std::ofstream o(getLocalFileName(url));
	o << s;
	return s;
}

std::string getLocalFileName(std::string const &url) {
	std::string s;
	char c;
	for (auto p = url.c_str(); (c = *p) != 0; p++) {
		if (strchr("/:?", c)) {
			c = '_';
		}
		s += c;
	}
	return storeFolder+"/" + s + ".html";
}

std::string readLocalFile(std::string const &url) {
	return fileGetContent(getLocalFileName(url));
}

std::string spanClass(std::string const &s) {
	return "<span class=\"" + s + "\">";
}

TagData getTagContent(std::string const &s, size_t startpos,
		std::string const &tag) {
	auto p = s.find(tag, startpos);
	if (p == std::string::npos) {
		return {false};
	}
	p += tag.length();
	auto p1 = s.find('<', p);
	assert(p1 != std::string::npos);
	return {true,s.substr(p, p1 - p),p1+1};
}

VTagData getTagsContent(std::string const &s, size_t startpos,
		VString const &vtag) {
	VTagData v;
	TagData t;
	size_t p = startpos;
	for (auto const &ta : vtag) {
		t = getTagContent(s, p, ta);
		v.push_back(t);
		if (!t.found) {
			break;
		}
		p = t.pos;
	}
	return v;
}

std::string timeString(clock_t begin, bool total/*=false*/) {
	return (total ? "totalTime" : "time") + format("=%.2lf", timeElapse(begin));
}

void init() {
	//https://curl.se/libcurl/c/multithread.html
	curl_global_init(CURL_GLOBAL_ALL);
}

void deinit() {
	curl_global_cleanup();
}


//many modification https://cpp.hotexamples.com/examples/-/-/zipOpenNewFileInZip/cpp-zipopennewfileinzip-function-examples.html
void addToZip(const char* archivePath, const char* path, const char* content, uint32_t length){
	int result = ZIP_OK;

	//int appendMode = fileExists(archivePath) ? APPEND_STATUS_ADDINZIP : APPEND_STATUS_CREATE;
	int appendMode = APPEND_STATUS_CREATE;

	zipFile archive = zipOpen(archivePath, appendMode);

	//aslov time of file in achive
	time_t rawtime;
	time(&rawtime);
	zip_fileinfo zfi;
	zfi.dosDate=0;
	zfi.tmz_date=*localtime(&rawtime);
	zfi.internal_fa=0;
	zfi.external_fa=0;

	result = zipOpenNewFileInZip4(
    		archive, path, &zfi,
    		nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, Z_DEFAULT_COMPRESSION,
    		0, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, NULL, 0, 0, 0);


	if (result != ZIP_OK)
	{
		printel("Unable to add new file to zip archive");
		return;
	}

	result = zipWriteInFileInZip(archive, content, length);
	if (result != ZIP_OK)
	{
		printel("Unable to write file data to zip archive");
		return;
	}
	result = zipCloseFileInZip(archive);
	if (result != ZIP_OK)
	{
		printel("Unable to close file in zip archive");
		return;
	}
	result = zipClose(archive, 0 /* global comment */);
	if (result != ZIP_OK)
	{
		printel("Unable to close zip archive");
		return;
	}
}

void mzipFile(std::string const &filepath){
	std::string s=fileGetContent(filepath, 1);
	auto zip=getFileInfo(filepath, FILEINFO::SHORT_NAME)+".zip";
	auto pathInsizeZip=getFileInfo(filepath, FILEINFO::NAME);
	addToZip(zip.c_str(), pathInsizeZip.c_str(), s.c_str(), s.length());
}

/*
std::pair<std::string, bool> downloadFileInner1(std::string const &url) {
	GFile *f = g_file_new_for_uri(url.c_str()); //f is always not null
	gsize length;
	char *content = NULL;
	GError *error;
	error = NULL;
	bool b = g_file_load_contents(f, NULL, &content, &length, NULL, &error);
	if (!b) {
		printel(error->code,error->message)
		g_error_free(error);
		return {"",false};
	}
	std::string s(content, length); //content is not null terminated so create std::string
	g_free(content);
	g_object_unref(f);

//	std::ofstream o(getLocalFileName(url));
//	o << s;
	return {s,true};
}

//add urlmon to linker libs
#include <windows.h>
std::pair<std::string, bool> downloadFileInner2(std::string const &url) {
	auto path = getLocalFileName(url);
	HRESULT res = URLDownloadToFile(NULL, url.c_str(), path.c_str(), 0, NULL);
	std::string s;
	if (res == S_OK) {
		std::ifstream t(path, std::ios_base::binary);
		std::stringstream buffer;
		buffer << t.rdbuf();
		s = buffer.str();
		return {s,true};
	} else if (res == E_OUTOFMEMORY) {
		printeln("Buffer length invalid, or insufficient memory");
	} else if (res == INET_E_DOWNLOAD_FAILURE) {
		printeln("URL is invalid [%s][%s]", url.c_str(), path.c_str());
	} else {
		printeln("Other error: %x", res);
	}
	return {"",false};
}
*/
