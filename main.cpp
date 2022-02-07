#include "src/Response/Response.hpp"
#include <sys/stat.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

Location newLocation(std::string path, vs methods, std::pair<size_t,std::string> redirect, std::string root, std::string index, bool autoindex)
{
	Location loc;
	loc.path = path;
	loc.accepted_methods = methods;
	loc.redirect = redirect;
	loc.root = root;
	loc.index = index;
	loc.autoindex = autoindex;
	return loc;
}

ServerCnf newServer(std::string host, size_t port, vs server_names, Locations locs)
{
	ServerCnf srv;
	srv.host = host;
	srv.port = port;
	srv.server_names = server_names;
	srv.locs = locs;
	return srv;
}

int main()
{
	std::vector<ServerCnf>	srvs;
	Locations				ls;
	Headers					hs;
	vs						sns;
	Response				res;
	struct sockaddr_in		addr;

	std::string html = "/Users/hrhirha/Desktop/webserv/www";

	// server 0
	ls.push_back(newLocation("/", vs(), std::make_pair(444,""), "", "", false));
	sns.push_back("");
	srvs.push_back(newServer("127.0.0.1", 8000, sns, ls));
	//////////
	ls.clear();
	hs.clear();
	sns.clear();
	// server 1
	ls.push_back(newLocation("/", vs(), std::make_pair(0,""), html, "index.html", true));
	// ls.push_back(newLocation("/dir0/index.html", vs(), std::make_pair(0,""), html, "", false));
	ls.push_back(newLocation("/dir0/", vs(), std::make_pair(0,""), html, "", true));
	ls.push_back(newLocation("/uploads/", vs(1, "POST"), std::make_pair(0,""), html, "", false));
	ls.push_back(newLocation("/dir0/dir00/", vs(), std::make_pair(307,"https://google.com"), html, "index.html", false));
	ls.push_back(newLocation(".php", vs(), std::make_pair(0,""), html, "", false));
	sns.push_back("localhost");
	srvs.push_back(newServer("127.0.0.1", 8000, sns, ls));
	//////////
	ls.clear();
	hs.clear();
	sns.clear();
	// server 2
	ls.push_back(newLocation("/", vs(), std::make_pair(0,""), html+"/dir1", "f10", false));
	sns.push_back("test.com");
	sns.push_back("test.net");
	srvs.push_back(newServer("127.0.0.1", 8000, sns, ls));
	/////////

	// Request
	hs.insert(std::make_pair("Host", "localhost"));
	// hs.insert(std::make_pair("Content-Type", "multipart/form-data; boundary=---------------------------144761614340942986943850864353"));
	// hs.insert(std::make_pair("Content-Type", "application/x-www-form-urlencoded"));
	// hs.insert(std::make_pair("Content-Length", "403")); // body_upload.txt
	// hs.insert(std::make_pair("Content-Length", "308")); // multipart_body.txt
	// hs.insert(std::make_pair("Content-Length", "34")); // xwww_body.txt
	Request req = {"GET", "/", "", "HTTP/1.1", hs, ""};

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//////////

	std::cout << "--------------------------------------------------------------\n";
	while (1)
	{
		// std::cout << "serving clients\n";
		if (res.build(req, srvs, addr))
		{
			std::cout << res.get();
		}
		if (res.done()) break ;
		usleep(1e3);
	}
	std::cout << "--------------------------------------------------------------\n";

	// for (int i = 3; i <= 1025; i++)
	// {
	// 	int fd = open(("tmp/" + std::to_string(i)).c_str(), O_CREAT | O_RDWR, 0644);
	// 	std::cout << "fd = " << fd << '\n';
	// 	if (fd == -1)
	// 	{
	// 		std::cout << "errno = " << errno << "\n";
	// 		break ;
	// 	}
	// }

/*
	struct stat buf;
	std::string path = "/Users/hrhirha/Desktop/webserv/www/index.html";
	if (stat(path.c_str(), &buf) == 0 && (buf.st_mode & S_IFMT) == S_IFREG)
	{
		int fd;
		std::cout << "file found" << std::endl;
		path = "/Users/hrhirha/Desktop/webserv/www/dir0/";
		if ((fd = open(path.c_str(), O_RDONLY)) == -1)
			std::cout << "Error (open()): " << errno << std::endl;
		else
		{
			std::cout << "No Error (open()): " << errno << std::endl;
			close(fd);
		}
	}
	else
		std::cout << "Error (stat()): " << errno << std::endl;
	std::cout << "EACCES: " << EACCES << std::endl; 	// permission denied for a path component
	std::cout << "EFAULT: " << EFAULT << std::endl; 	// invalid path
	std::cout << "EIO: " << EIO << std::endl; 			// I/O error while reading or writing to the file system
	std::cout << "ENOENT: " << ENOENT << std::endl;		// file does not exist
	std::cout << "ENOTDIR: " << ENOTDIR << std::endl;	// a component of the path is not a directory
	// std::cout << sizeof(struct stat) << "\n";

	time_t clock;
	struct tm tm;

	time(&clock);
	tm = *gmtime(&clock);
	std::cout << tm.tm_hour << ":" << tm.tm_min << ":" << tm.tm_sec << " - " << tm.tm_mday << "/" << tm.tm_mon+1 << "/" << 1900+tm.tm_year << std::endl;
	std::cout << asctime(&tm) << std::endl;
*/
	// std::cout << "tablesize = " << getdtablesize() << "\n";
	// while (1)
	// {
	// 	usleep(10);
	// }
}
