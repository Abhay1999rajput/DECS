#include "http_server.hh"
#include<bits/stdc++.h>
#include <vector>

#include <sys/stat.h>

#include <fstream>
#include <sstream>
#include<string>
vector<string> split(const string &s, char delim) {
  vector<string> elems;

  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    if (!item.empty())
      elems.push_back(item);
  }
 // cout<<"Abhay";
  return elems;
}

HTTP_Request::HTTP_Request(string request) {
  vector<string> lines = split(request, '\n');
  vector<string> first_line = split(lines[0], ' ');
  this->url=  first_line[1];
  //cout<<"Abhay";
  this->method = first_line[0];

  this->HTTP_version = "1.0"; // We'll be using 1.0 irrespective of the request

 //cout<<first_line[0]<<" "<<first_line[1];

  if (this->method != "GET") {
    cerr << "Method" << this->method << " not supported" << endl;
    exit(1);
  }
}

HTTP_Response *handle_request(string req) {
  //cout<<req;
  HTTP_Request *request = new HTTP_Request(req);
//cout<<"ABhay";
  HTTP_Response *response = new HTTP_Response();

  string url = string("html_files") + request->url;

   response->HTTP_version = "1.0";

  struct stat sb;
   
   // cout<<url;
  if (stat(url.c_str(), &sb) == 0) // requested path exists
  {
    //stat(url, &sb);
    //
    response->status_code = "200";
   response->status_text = "OK";
   response->content_type = "text/html";
    if (S_ISDIR(sb.st_mode)) {
      /*
      In this case, requested path is a directory.
      TODO : find the index.html file in that directory (modify the url
      accordingly)
      */
      //cout<<"ABhay";
      url = url+"/index.html";
    }

  string line,Data;
  ifstream myfile (url);
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      //cout << line << '\n';
      Data+=line;
      Data+="\n";
    }
    response->content_length= to_string(Data.length());
    myfile.close();
  }
    //cout<<Data;
    // char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
    
    /*
    TODO : open the file and read its contents
    */
   
  
    response->body=Data;
    
   response->body= response->get_string();
    //cout<<papa<<"na";
    /*
    TODO : set the remaining fields of response appropriately
    */
  }
      
  else {
   response->status_code = "404";
   response->status_text = "Not Found";
   response->content_type = "text/html";
   url = "errorfile.html";
  string line,Data;
  ifstream myfile (url);
  
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      //cout << line << '\n';
      Data+=line;
      Data+="\n";
    }
    response->content_length= to_string(Data.length());
    myfile.close();
  }
    //cout<<Data;
    // char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
    
    /*
    TODO : open the file and read its contents
    */
  response->body=Data; 
  
  string str;

  //struct response str;

  str+="HTTP/";
  str+=response->HTTP_version;
  str+=" ";
  str+=response->status_code;
  str+=" ";
  str+=response->status_text;
  str+="\n";
  str+="Content-Type:";
  str+=response->content_type;
  str+="\n";
  str+="Content_Length:";
  str+=response->content_length;
  str+="\n";
  str+=response->body;
  str+="\n\n";

  // response->status_code = "404";
response->body= str;
 
   /*
    TODO : set the remaining fields of response appropriately
    */
  }
  

  delete request;

  return response;
}

string HTTP_Response::get_string() {
  /*
  TODO : implement this function
  */
  string str;

  //struct response str;

  str+="HTTP/";
  str+=HTTP_version;
  str+=" ";
  str+=status_code;
  str+=" ";
  str+=status_text;
  str+="\n";
  str+="Content-Type:";
  str+=content_type;
  str+="\n";
  str+="Content_Length:";
  str+=content_length;
  str+="\n";
  str+=body;
  str+="\n\n";

   
 return str;
}
