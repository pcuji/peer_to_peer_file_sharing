#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

using namespace std;
vector<string> p1vec;
vector<string> p2vec;
vector<string> p3vec;

int portR2 = 60002;     //search
int root_peer;
int client_node;
const char *my_t, * my_file ;
char buf_serv_list[1024];                //contains the peer serching for file
string collect_serv_search;
string str_f, file_in_serch;
//char collect_serv_search;

void *register_run(void *dummyPt)
{
    cout << "Hellor form register_run Thread No: " << pthread_self() << endl;
    char test[300];
    bzero(test, 300);
    string token,token2;
    std::string delimiter = ";";
    size_t pos = 0;

    //pthread_mutex_lock(&mutex);
    read(connFd, test, 300);
    string tester (test);


    p1vec.push_back(tester);
    //pthread_mutex_unlock(&mutex);

    cout << "Looping through vector with register files" << endl;
    for(std::vector<string>::size_type i = 0; i != p1vec.size(); i++) {
        cout << p1vec[i]  << endl;
    }
    cout << "Peer ID: " << token2 << " Sucssesfull register the files\n" << endl;
    root_peer = stoi(token2);

    cout << "\nClosing thread " << endl;
    /*close(socket) you can close() the listening sockfd in order to prevent more incoming connections on
    */
    pthread_exit(0); //thread created by port_runable
}

//****************

void  *linear_search(  void *vptr_value){
    str_f.clear();
    char *str = (char *)vptr_value;
    cout << "linear_search w/file: " << str << "  on server portNo: "<< portR2 << endl;
    string str3(str);
    //cout << "the string that I just pass is " << str3 << "\n";
    //string mystr,token2 ,str_f,save_str;
    string mystr,token2 ,save_str;
    std::string delimiter = ",";
    vector<string> new_vec;
    size_t pos;
    int flag;
        cout << "File: "<< mystr << "\n";
        cout << "Deleting token?: "<< token2 << "  savestr is :" <<save_str<< "  the flag is:" <<flag<< "\n";


        if (strcmp(  mystr.c_str(), str3.c_str())==0){

            cout <<"I am comparing the two strings "<< mystr << " and " << str3<< "\n";
            str_f+= save_str + "&" ;

        }
    }

    if (write(serchFd, str_f.c_str(), 300) == -1){
     perror("send");

    }
    //i do not wnat to use the file descriptor to respont yet
    //i need to look for other servrs
    //if (write(serchFd, str_f.c_str(), 300) == -1){
     //perror("send");
     //close(connFd);
    //}
    //collect_serv_search = str_f.c_str();
    cout <<"the result of the concatenated string is "<< str_f << "\n";

    pthread_exit(0); //killing thread called by port_runableS
}

//*****************

void *look_up_indiv_ser(void *vptr_value){
  char *str = (char *)vptr_value;
  cout << "Processing inx server port #:  " << str << "\n";
  string str3(str);
  int port_search = stoi(str3);
  int portNo = port_search;
  int numb;
  char buf_look[300];
  struct sockaddr_in svrAdd;
  struct hostent *server;
  //create client socket by two file descriptors
  int lookFd_search = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(lookFd_search < 0){
      cerr << "Cannot open socket" << endl;
      return 0;
  }
  server = gethostbyname("localhost");

  bzero((char *) &svrAdd, sizeof(svrAdd)); //bzero() sets all values in a buffer to zero. It takes two arguments, the first is a
  //pointer to the buffer and the second is the size of the buffer. Thus, this line initializes serv_addr to zeros.
  svrAdd.sin_family = AF_INET;
  bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
  svrAdd.sin_port = htons(portNo);        //serverAddr.port number = host to network short
  //we need to specify the server and port to connect to using echoserver.sin_addr.s_addr and
  int checker2 = connect(lookFd_search,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
  //   int sockfd, struct sockaddr *serv_addr, int addrlen= size of the addr
  //sockfd is our friendly neighborhood socket file descriptor, as returned by the socket() call
  if (checker2 < 0){
      cerr << "Cannot connect!" << endl;
      return 0;
  }
  write(lookFd_search, file_in_serch.c_str(), 300);
  write(lookFd_search, str3.c_str(),1024);
  string s;
  stringstream out;
  out << portR2;
  s = out.str();
  write(lookFd_search,s.c_str(), 300);

   //therad called by sequential_sev_search

}

void  *sequential_sev_search(void *vptr_value){
//int sequential_sev_search(string serv_l ){
  char *s_file = (char *)vptr_value;
  cout << "#### PROCESSING SEQUENTIAL LIST w/file: " << s_file << "\n";
  string str_file(s_file);
  string str_serv(buf_serv_list);
  string str_results, save_str,token;
  cout << "LOOPING Idx servers addr list: "<< str_serv <<endl;
  string delimiter = ",";
  int pos = 0;
  int serv_port;
  my_file = str_file.c_str();
  //loop over rest of servers
  while ((pos = str_serv.find(delimiter)) != std::string::npos) {
      token = str_serv.substr(0, pos);
      serv_port = stoi(token);
      my_t=token.c_str();
      if (serv_port ==portR2){//tis server
        pthread_t this_servid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&this_servid, &attr,linear_search,(void *)my_file);
        pthread_join(this_servid, NULL);
        cout << "##### linear_search in LOCAL_SERVER"<< endl;
      }
      else {
        pthread_t secid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&secid, &attr,look_up_indiv_ser,(void *)my_t);
        pthread_join(secid, NULL);
      }

      if (str_f.size() != 0){
        str_results += str_f;
      }
      str_f.clear();        //if they do not connect they will add up
      //collect_serv_search.clear();
      //cout << "the token found this time is: "<< token << "\n";
      cout << "the str_results this time is: " << str_results<< "\n";
      cout << "\n";

      str_serv.erase(0, pos + delimiter.length());
  }
  cout << "the last server is: "<< str_serv << "\n";
  serv_port = stoi(str_serv);
  my_t=str_serv.c_str();
  if (serv_port ==portR2){//tis server
    pthread_t this_servid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&this_servid, &attr,linear_search,(void *)my_file);
    pthread_join(this_servid, NULL);
    cout << "##### linear_search in LOCAL_SERVER"<< endl;
  }
  else {
    pthread_t secid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&secid, &attr,look_up_indiv_ser,(void *)my_t);
    pthread_join(secid, NULL);
  }

  if (str_f.size() != 0){
    str_results += str_f;
  }
  str_f.clear();

  if (write(serchFd, str_results.c_str(), 1024) == -1){
    perror("send");
  }

  str_results.clear();
  //look_up_indiv_ser();
  //str_results += collect_serv_search ;
  pthread_exit(0); //killing thread called by port_runableS
  //return 0;
}


void *Port_RunnableS( void *vptr_value){
    int value = *((int *)vptr_value);
    //cout << "Hellor form register_run Thread No: " << pthread_self() << " and the port is "<< value << endl;
    cout << "Search_run Thread No: " << pthread_self() << " and the port is "<< value << endl;

    //pthread_mutex_unlock(&mutex);

    /*
     //2. create socket
     listenFd = socket(AF_INET, SOCK_STREAM, 0);
     int listen(int sockfd, int backlog);
     sockfd is the usual socket file descriptor from the socket() system call. backlog is the number
     connections allowed on the incoming queue. What does that mean? Well, incoming connections are goingtowait
     in this queue until you accept() them (see below) int socket(int domain, int type, int protocol)
     */
    listenFd_search = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*
     PF_INET just means it uses IP (always used)
     The value returned is a socket handle, which is similar to a file
     handle

    */

    /*we need to specify the server and port to connect to using server.sin_addr.s_addr and.
    allows not to wait when unbinding
    */
    int yes=1;
    if (setsockopt(listenFd_search,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }

    //3 .bind socket
    if(bind(listenFd_search, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0){
        cout << " Thread No: " << pthread_self() << " and the port is "<< value << endl;
        cerr << "Cannot bind" << endl;
        return 0;
    }

    //4.  the 5 is the number of conections allowed in the incomming queue
    listen(listenFd_search, 5);
    len = sizeof(clntAdd);
    //Ok, time for a change of pace. What if you don't want to connect to a remote host. Say, just for kicks,
    //that you want to wait for incoming connections and handle them in some way. The process is two step:
    //first you listen(), then you accept() (see below.)

    char buf_r[300],buf_p[300];
    //char buf_serv_list[1024];                //contains the peer serching for file
    //char *str
    //bzero(buf_f ,300);
    if (portNo == portR2){
        cout << "##### searching " << endl;
        //********************

        while (true){
            socklen_t len = sizeof(clntAdd);
            cout << "Listening" << endl;
            /*listenFd is the listen()ing socket descriptor
             this is where client connects. svr will hang in this mode until client conn
             addr will usually be a pointer to a local struct sockaddr_storage.
             This is where the information about the incoming connection will go (and withyou
             can determine which host   calling you from which port)
             */
            //5.
            //two things will happen here either Im requesting file search form my peer
            //or from a server

            serchFd = accept(listenFd_search, (struct sockaddr *)&clntAdd, &len);
            //connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);
            cout<< "processing your request in the run_eaching  side" << endl;
            if (serchFd < 0){
                cerr << "Cannot accept connection" << endl;
                return 0;
            }
            else{
                cout << "Connection successful" << endl;
                bzero(buf_r, 300);
                //read(connFd, buf_r, 300);
                //cout << "I am wainting for you responst to be 2";
                read(serchFd, buf_r, 300);
                string help_file(buf_r);
                file_in_serch = help_file;
                bzero(buf_serv_list, 1024);
                int numbytes;
                numbytes = read(serchFd, buf_serv_list, 1024);
                string str3(buf_serv_list);
                bzero(buf_p, 300);
                read(serchFd, buf_p, 300);
                string str_peer(buf_p);
                client_node = stoi(str_peer);
                cout << "Client_node is port # "  << client_node << endl;

                //buf_serv_list[numbytes] = '\0';
                cout << "SERVERS ADDRS RECEIVED :" << buf_serv_list<< endl;
                //cout << "the client node is requesting the serch is automatic or peer "<< client_node<< endl;



                cout << "AUTO SEARCH TO OTHER SERVERS AROUND neighborhood"<< endl;
                str_f.clear();
                pthread_t Autoid;
                pthread_attr_t attr;
                pthread_attr_init(&attr);
                pthread_create(&Autoid, &attr,linear_search,(void *)buf_r);
                pthread_join(Autoid,NULL);


      //********************
                //you can add a not allow input prehaps
                //if (strcmp( buf_r ,"file1")==0){
                    //char *str
                    //str -> string2;

                    //pthread_t tid;
                    //pthread_attr_t attr;
                    //pthread_attr_init(&attr);

                    //pthread_create(&tid, &attr,sequential_sev_search,(void *)buf_serv_list);
                    //pthread_create(&tid, &attr,sequential_sev_search,(void *)buf_r);
                    //pthread_join(tid, NULL);
                  cout << "I just create a thead to do the search\n";
                //}

            }
            cout << "I close the listenFd_serch file descriptor";
            close(serchFd);
        }
    }
    cout << "*********do not get out yet I might need to do some work ***********" << endl;
    pthread_exit(0);

}



//*****************Thread Register running ***************


void *Port_RunnableR( void *vptr_value){
    int value = *((int *)vptr_value);
    cout << "Register_run Thread No: " << pthread_self() << " and the port is "<< value << endl;
    //pthread_mutex_unlock(&mutex);
    int pId, portNo ,listenFd;  //portNo allows to connect
    socklen_t len;              //store size of the address
    bool loop = false;
    struct sockaddr_in svrAdd, clntAdd;
    pthread_t id_t[3];

    //1. is this getting the address
    portNo = value;
    /*
    //2. create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    int listen(int sockfd, int backlog);
    sockfd is the usual socket file descriptor from the socket() system call. backlog is the number
    connections allowed on the incoming queue. What does that mean? Well, incoming connections are goingtowait
    in this queue until you accept() them (see below) int socket(int domain, int type, int protocol)
    */
    listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenFd < 0){
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    bzero((char*) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);
    /*Basically, you'll want to convert the numbers to Network Byte Order before they go out on the wire, and convert them to Host Byte Order asthey come in off the wire.
     Allows not to wait when unbinding
    */
    int yes=1;
    if (setsockopt(listenFd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }

    //3 .bind socket
    if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0){
        cout << " Thread No: " << pthread_self() << " and the port is "<< value << endl;
        cerr << "Cannot bind" << endl;
        return 0;
    }
    //4.  the 5 is the number of conections allowed in the incomming queue
    listen(listenFd, 5);
    len = sizeof(clntAdd);

    //Ok, time for a change of pace. What if you don't want to connect to a remote host. Say, just for kicks,
    //that you want to wait for incoming connections and handle them in some way. The process is two step:
    //first you listen(), then you accept() (see below.)

    char buf_r[300];
    bzero(buf_r ,300);
    int noThread = 0;
    if (portNo == portR){
        cout << "##### Process Registering " << endl;

        while (true){
        //while (noThread < 3){
            socklen_t len = sizeof(clntAdd);
            cout << "Listening" << endl;
            /*listenFd is the listen()ing socket descriptor
            this is where client connects. svr will hang in this mode until client conn
            addr will usually be a pointer to a local struct sockaddr_storage.
            This is where the information about the incoming connection will go (and with it you
            can determine which host   calling you from which port)
            */

            //5.
            connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);
            if (connFd < 0){
                cerr << "Cannot accept connection" << endl;
                return 0;
            }
            else{
                cout << "Connection successful" << endl;

                //bzero(buf_r, 300);
                //read(connFd, buf_r, 300);
                //if (strcmp( buf_r ,"1")==0){
                    pthread_t tid;
                    pthread_attr_t attr;
                    pthread_attr_init(&attr);
                    pthread_create(&tid, &attr,register_run,NULL);
                    //pthread_create(&id_t[noThread], &attr,register_run,NULL);
                    pthread_join(tid, NULL);

                //}

            }

            //************
            close(connFd);
            //*********


            noThread++;
        }
        //** while
        //for(int i = 0; i < 3; i++){
            //pthread_join(id_t[i], NULL);
            //cout << "id_t joind Thread No: " << id_t[i] << endl;
            //pthread_join(threadA[i], NULL);
        //}
        }
    cout << "*********do not get out yet I might need to do some work ***********" << endl;
    pthread_exit(0); //register request thread

}



int main(int argc, char * argv[]) {

    RegisterRequestThread();
    SearchRequestThread();



    pthread_join(stid, NULL);
    pthread_join(tid, NULL);

    std::cout << "Hello, World!\n";
    return 0;
}
