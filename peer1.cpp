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
#include <strings.h>

//hola
#define BUF_SIZE 1024
using namespace std;
vector<string> v_servport;
size_t getFilesize(const std::string& filename);
static int listenFd,listenFd_search,listenFd_download;

//server behaivor for dowing work as downloading
static int downloadFd;
//just like peer behaivor but for download
static int listFd_mydownload; //use: obtain()

pthread_t dtid;             //global download thread id
int portD = 60032;          // download port
int port_search;


//new additions

string str_f, file_in_serch;
const char *my_t, * my_file ;


//Both the indexing server and a peer server should be able to accept multiple client requests at the same time. This could be easily done using threads. Be aware of the thread synchronizing issues to avoid inconsistency or deadlock in your system.
//specifies a file name with the indexing server using "lookup"
//concttenated string and with all indexing peers
// and maybe 3 enveded loops going thtough
//as a server As a server, the peer waits for requests from other peers and sends the requested file when receiving request.
//void *Port_Run_Download( void *vptr_value){
//*****************

void obtain() {
    string infofile,port_str,file_str;

    int numbytes,new_numbytes;
    char buf[300];
    char s[300], get_op[300];
    struct sockaddr_in svrAdd;
    struct hostent *server;


    server = gethostbyname("localhost");
    if(server == NULL){
        cerr << "Host does not exist" << endl;
    }
    bzero((char *) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    svrAdd.sin_port = htons(portNo);
    int checker = connect(listFd_mydownload,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    if (checker < 0){
        cerr << "Cannot connect!" << endl;
    }
    cout << "downloading.............. ";
    char tmp[300];
    strncpy(tmp, infofile.c_str(), infofile.length());
    write(listFd_mydownload, tmp, strlen(get_op));
    //receiving data
    //downloader .............
    //*************************
    char buf2[1024];
    int flag = 2;


    //read buffer size

    buf2[numbytes] = '\0';
    printf("client: received '%s'\n",buf2);

    size_t fSize, remaining;
    if(strstr(buf2,"SIZE")!=NULL)
    {
        strtok(buf2," ");
        fSize=strtol(strtok(NULL," "),0,10);
    }
    remaining=fSize;
    cout << "the remaining value and the fSize are: "<< remaining<< fSize<< "\n" ;
    bzero(buf2,1024);
    if ((numbytes = read(listFd_mydownload,buf2,1024)) == -1){
        perror("recv");
        exit(1);
    }
    size_t limit;
    if(strstr(buf2,"SIZE")!=NULL)
    {
        strtok(buf2," ");
        fSize=strtol(strtok(NULL," "),0,10);
    }
    limit=fSize;
    cout << "the limit this time is: "<< limit<< "\n" ;

    std::string output_file_name = std::string(file_str+".tokens");
    std::ofstream output_file(output_file_name.c_str());
    numbytes =0;
    bzero(buf2,1024);

    int flag2 =0;

    //end of dowloading
    cout << "display file 'foo'\n" ;
    close(listFd_mydownload);

}

void *new_look_up_indiv_ser(void *vptr_value){
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
  if(server == NULL){
      cerr << "Host does not exist" << endl;
      return 0;
  }
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
  
  write(lookFd_search,s.c_str(), 300);

  if ((numb = read(lookFd_search,buf_look,300)) == -1){
      perror("recv");
      exit(1);
  }
  buf_look[numb] = '\0';
  cout << "The returned search is:  " << buf_look<< "\n";
  string helper(buf_look);
  str_f = helper ;
  //cout << "the returned value from server is "<< str_f<< endl;
  close(lookFd_search);
  cout << "---CLOSING neighborhood PORT CONNECTION"<< endl;
  pthread_exit(0);    //therad called by sequential_sev_search

}



void *new_sequential_sev_search(void *vptr_value){
  cout << "IM DOING sequential server search in the neighborhood\n";
  char *s_file = (char *)vptr_value;
  cout << "#### PROCESSING SEQUENTIAL LIST w/file: " << s_file << "\n";
  string str_file(s_file);

  //string str_serv(buf_serv_list);
  string str_serv(v_servport[8]);
  string str_results, save_str,token;
  cout << "LOOPING Idx servers addr list: "<< str_serv <<endl;
  string delimiter = ",";
  int pos = 0;
  int serv_port;
  my_file = str_file.c_str();
  int flag2=0;
  //loop over rest of servers
  while ((pos = str_serv.find(delimiter)) != std::string::npos) {
      token = str_serv.substr(0, pos);
      serv_port = stoi(token);
      my_t=token.c_str();


      pthread_t secid;
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_create(&secid, &attr,new_look_up_indiv_ser,(void *)my_t);
      pthread_join(secid, NULL);

      if (str_f.size() != 0){
        str_results += str_f;
        flag2 =1;
        break;
      }
      str_f.clear();        //if they do not connect it will keep adding
      //collect_serv_search.clear();
      //cout << "the token found this time is: "<< token << "\n";
      cout << "the str_results this time is: " << str_results<< "\n";
      cout << "\n";
      str_serv.erase(0, pos + delimiter.length());
  }

  serv_port = stoi(str_serv);
  my_t=str_serv.c_str();

  if (flag2 ==0){
    cout << "the last server is: "<< str_serv << "\n";
    pthread_t secid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&secid, &attr,new_look_up_indiv_ser,(void *)my_t);
    pthread_join(secid, NULL);
    if (str_f.size() != 0){
      str_results += str_f;
    }
  }
  //serv_port = stoi(str_serv);
  //my_t=str_serv.c_str();

  //pthread_t secid;
  //pthread_attr_t attr;
  //pthread_attr_init(&attr);
  //pthread_create(&secid, &attr,new_look_up_indiv_ser,(void *)my_t);
  //pthread_join(secid, NULL);
  //if (str_f.size() != 0){
    //str_results += str_f;
  //}
  cout << "The result for the search is : " << str_results<< "\n";
  //if (write(serchFd, str_results.c_str(), 1024) == -1){
    //perror("send");
  //}
  str_results.clear();
  pthread_exit(0); //killing thread called by port_runableS

}

//collecter of the info
int  look_up2(){
  cout << "IM DOING look_up2 funtion\n";
//***************
  char N_s[300];
  char N_I[300];
  //int portNo = port_search;
  cout << "Enter file as: file10\n";
  cout << "serch for file name: " ;
  bzero(N_s, 300);
  cin.getline(N_s, 300);
  string help_file(N_s);
  file_in_serch = help_file;
  //2.
  cout << "enter your peer id: " ;    //note i am not doing anything
                                      //w/ id at the first input
  bzero(N_I, 300);
  cin.getline(N_I, 300);

  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  //pthread_create(&tid, &attr,sequential_sev_search,(void *)buf_serv_list);
  pthread_create(&tid, &attr,new_sequential_sev_search,(void *)N_s);
  pthread_join(tid, NULL);
  cout << "I just create and finish a thead to do the search\n";
  return 0;
  }




int look_up(){
    char s[300];
    int numbytes;
    char buf[300];
    //int portNo = 60006;             //look the server port search number
    int portNo = port_search;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;
    //create client socket by two file descriptors
    listenFd_search = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenFd_search < 0){
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    server = gethostbyname("localhost");
    if(server == NULL){
        cerr << "Host does not exist" << endl;
        return 0;
    }
    bzero((char *) &svrAdd, sizeof(svrAdd)); //bzero() sets all values in a buffer to zero. It takes two arguments, the first is a
    //pointer to the buffer and the second is the size of the buffer. Thus, this line initializes serv_addr to zeros.
    svrAdd.sin_family = AF_INET;
    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    svrAdd.sin_port = htons(portNo);        //serverAddr.port number = host to network short
    //we need to specify the server and port to connect to using echoserver.sin_addr.s_addr and
    int checker2 = connect(listenFd_search,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    //   int sockfd, struct sockaddr *serv_addr, int addrlen= size of the addr
    //sockfd is our friendly neighborhood socket file descriptor, as returned by the socket() call
    if (checker2 < 0){
        cerr << "Cannot connect!" << endl;
        return 0;
    }
    cout << "Enter file as: file10\n";
    cout << "serch for file name: " ;
    bzero(s, 300);
    cin.getline(s, 300);
    write(listenFd_search,s, strlen(s));
    //cout << "enter your peer id: "<< endl;
    //bzero(s, 301);
    //cin.getline(s, 300);
    //write(listenFd_search,s, strlen(s));
    string line = v_servport[8];
    cout << "PASSING LIST INDEX SER ADDR " << line <<endl;

    write(listenFd_search,v_servport[8].c_str(), 1024);
    cout << "enter your peer id: " ;
    bzero(s, 300);
    cin.getline(s, 300);
    write(listenFd_search,s, strlen(s));

     //if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
     if ((numbytes = read(listenFd_search,buf,300)) == -1){
         perror("recv");
         exit(1);
     }
     buf[numbytes] = '\0';
    cout << "The result for the search is:  " << buf << "\n";
    close(listenFd_search );
    return 0;
}

int get_port_reg(int serv_num){
  string token2 , mystr;
  size_t pos;
  mystr = v_servport[serv_num];
  pos = 0;
  std::string delimiter = ",";

  istringstream iss(mystr);
  getline(iss, token2, ',');
  int portNo_regis = stoi(token2);

  while ((pos = mystr.find(delimiter)) != std::string::npos) {
      token2 = mystr.substr(0, pos);
      //cout << "the token found this time is: "<< token2 << "\n";
      //if (!flag){
          //save_str = token2;
          //flag++;
      //}
      mystr.erase(0, pos + delimiter.length());
  }

  port_search =  stoi(mystr);
  //cout << "the port_search is "<< port_search<<endl;
  //cout << "the port_registar is " << portNo_regis<<endl;

  //istringstream iss(mystr);
  //getline(iss, token2, ',');
  //int portNo_regis = stoi(token2);
  return portNo_regis;

}

//pick aserver function
int pick_servfun(){
  cout<< "PICK WHICH SERVER WOULD YOU LIKE TO REGESTER WITH: \n" <<endl;
  cout<<"===================================================\n1 \n2 \n3 \n4 \n5 \n6 \n7 \n8\n "<<endl;
  char s[300], get_op[300];
  cout << "Enter server number: ";
  bzero(s, 300);
  bzero(get_op,300);
  cin.getline(s, 300);
  int port_registar;
  if(strcmp( s ,"1")==0){
    cout << "I picked 1"<<endl;
    port_registar = get_port_reg(0);
    cout << "REGISTERING WITH SERVER PORT " << port_registar << endl;
    return port_registar;
  }
  else if(strcmp(s, "2")==0){
    port_registar = get_port_reg(1);
    cout << "REGISTERING WITH SERVER PORT " << port_registar << endl;
    return port_registar;
  }

  else if(strcmp(s, "5")==0){
    port_registar = get_port_reg(4);
    cout << "REGISTERING WITH SERVER PORT " << port_registar << endl;
    return port_registar;
  }
  else if(strcmp(s, "6")==0){
    port_registar = get_port_reg(5);
    cout << "REGISTERING WITH SERVER PORT " << port_registar << endl;
    return port_registar;
  }
  else if(strcmp(s, "7")==0){
    port_registar = get_port_reg(6);
    cout << "REGISTERING WITH SERVER PORT " << port_registar << endl;
    return port_registar;
  }
  else if(strcmp(s, "8")==0){  // everything else that is not in the menu
    port_registar = get_port_reg(7);
    cout << "REGISTERING WITH SERVER PORT " << port_registar << endl;
    return port_registar;

  }
}




int reg_with_server(){

    //******************
    int numbytes;
    char buf[300];
    char s[300], get_op[300];

    int port_reg = pick_servfun();


    //int portNo=60008;
    int portNo = port_reg;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;
    //create client socket by two file descriptors
    //listenFd_search = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenFd < 0){
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    server = gethostbyname("localhost");
    if(server == NULL){
        cerr << "Host does not exist" << endl;
        return 0;
    }
    bzero((char *) &svrAdd, sizeof(svrAdd)); //bzero() sets all values in a buffer to zero. It takes two arguments, the first is a
    //pointer to the buffer and the second is the size of the buffer. Thus, this line initializes serv_addr to zeros.
    svrAdd.sin_family = AF_INET;
    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    svrAdd.sin_port = htons(portNo);        //serverAddr.port number = host to network short
    //we need to specify the server and port to connect to using echoserver.sin_addr.s_addr and
    int checker = connect(listenFd,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    //int checker2 = connect(listenFd_search,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    //   int sockfd, struct sockaddr *serv_addr, int addrlen= size of the addr
    //sockfd is our friendly neighborhood socket file descriptor, as returned by the socket() call
    if (checker < 0){
        cerr << "Cannot connect!" << endl;
        return 0;
    }
    cout << "Enter Files as PeerID,text;PeerID... : 00003,file1;00003,file3;00003,file4;00003,file5\n ";
    cout << "Enter : ";

    bzero(get_op,300);
    cin.getline(get_op,300);
    write(listenFd,get_op, strlen(get_op));
    cout << "closing listerFd at peer1\n" ;
    close(listenFd);
    return 0;
    //********************

}

//******************************
//server behaivor in the peer
//**************************
//****************uploader to the other peer requesting data************
size_t getFilesize(const std::string& filename) {
    struct stat st;
    if(stat(filename.c_str(), &st) != 0) {
        return 0;
    }
    return st.st_size;
}


void  *download(  void *vptr_value){
    char *str = (char *)vptr_value;
    cout << "the string that I just pass is " << str << "\n";
    string str3(str);
    cout << "the string that I just pass is " << str3 <<"\n" ;
    string mystr,token2,token;
    std::string delimiter = ";";
    string tester (str3);
    istringstream iss(tester);
    getline(iss, token, ';');
    int numbytes, new_numbytes;
    cout << "my file shoulf be what you are lookin for:  " << token << "\n";

    char buf2[BUF_SIZE];

    token= token+".txt";
    cout << "the token concatenated with .txt to download is " << token <<"\n";
    str3= str3+".txt";
    cout << "the str3 concatenated with .txt to download is " << str3;
    //responding
    int flag =2;
    size_t fSize, remainingR, remainingS;
    //FILE *fp = fopen(token,"rb");

    mystr ="SERVER PEER  UPLOADING FILE";
    while ( flag>= 0 ){
        if (write(downloadFd, mystr.c_str(), 300) == -1){
            perror("send");
        }
        flag--;
    }
    //get size of file

    fSize= getFilesize(token);
    remainingR=fSize;
    remainingS=fSize;
    cout << "The size of the file is \n"<< fSize;
    //send the size over
    bzero(buf2,1024);
    snprintf( buf2, BUF_SIZE, "SIZE %lu ", fSize);   //send file size

    if (write(downloadFd , buf2, fSize) == -1){
        perror("send");
    }
    cout<<"Starting Upload "<<endl;
    cout<<"Uploading..."<<endl;


    //downloading part
    //************
    //start fresh

    //sending limit
    bzero(buf2,1024);
    snprintf( buf2, BUF_SIZE, "SIZE %lu ", limit);   //send file size
    if (write(downloadFd , buf2, fSize) == -1){
        perror("send");
    }
    std::ifstream ifs (token.c_str(), std::ifstream::in);



    cout << "Exiting the download thread\n";
    pthread_exit(0);    //therad called by port_run_download

}

//*****************


///peer# creates a port for download conection

    //**************

}


void DownloadRequestThread(){   //FileDownload Request Thread and send the peer id to be downloaded

    cout << "option 1 from menu wake me up as a download thread \n";
    pthread_attr_t attr2;                                    //thread attribut
    pthread_attr_init(&attr2);
    pthread_create(&dtid, &attr2,Port_Run_Download,(void*)&portD);
    //pthread_join(stid, NULL);
    cout << "tread for download request created!!!   \n " << endl;

}




int main (int argc, char* argv[])
{
    int numbytes;
    char buf[300];
    cout <<"||========================================================================================||"<<endl;
    cout <<"||              Decentralized peer to peerFILE SHARING SYSTEM                             ||"<<endl;
    cout <<"||                   ========================================                             ||"<<endl;
    cout <<"||========================================================================================||"<<endl;

    cout << "reading config file" << endl;
    string line;
    std::ifstream myfile("config.txt");
    //vector<std::string> myLines;
    while (std::getline(myfile, line))
    {
      v_servport.push_back(line);
    }
    //cout << "Looping through vector v_servport" << endl;
    for(std::vector<string>::size_type i = 0; i != v_servport.size(); i++) {
        //cout << v_servport[i]  << endl;
    }
    //cout << "Peer ID: " << token2 << " Sucssesfull register the files\n" << endl;
    //control loop
    while(true){
        //list of options
    cout<<"============================================================================================\n";
        cout<<"Enter The Option :\n==================\n1. Registering the File \n \n2. Searching On Any IndxServer \n \n3. Downloading From Peer Server \n \n4. Exit\n"<<endl;
        char s[300], get_op[300];
        cout << "Enter stuff: ";
        bzero(s, 300);
        bzero(get_op,300);
        cin.getline(s, 300);

        if(strcmp( s ,"1")==0){
            reg_with_server();
            DownloadRequestThread();
            //write(listenFd,s, strlen(s));
            //cout << "Enter Files as PeerID,text;PeerID... : 00003,file1;00003,file3;00003,file4;00003,file5\n ";
            //cout << "Enter : ";
            //bzero(get_op,300);
            //cin.getline(get_op,300);
            //write(listenFd,get_op, strlen(get_op));
            continue;

        }
        else if(strcmp(s, "2")==0){
            cout << "serching server" << endl;
            look_up2 ();
            //look_up();
            //write(listenFd_search,get_op, strlen(get_op));
            continue;

        }
        else if(strcmp(s, "3")==0){
            cout << "downloading form peer" << endl;
            //DownloadRequestThread();
            obtain();
            //cout << "dispaly file foo" << "\n";

        }
        else if(strcmp(s, "4")==0){
            cout << " another option " << endl;
        }
        else if(strcmp(s, "4")==0){
            //cin.getline(get_op,300);
            //write(listenFd,s, strlen(s));
            //write(listenFd,get_op, strlen(get_op));
            cout << "exit out" << endl;
            exit(0);
        }
        else {  // everything else that is not in the menu
            //write(listenFd,s, strlen(s));
            cout<< "PLEASE ENTER A VALID OPTION! from the menu !"<< endl;
            //if ((numbytes = read(listenFd,buf,300)) == -1){
                //perror("recv");
                //exit(1);
            //}
            //buf[numbytes] = '\0';
            //printf("server says: received '%s'\n",buf);
        }
        //break;
    }

    pthread_join(dtid, NULL);   // waiting for killing download thread
    cout << "this tread should never die sice its the downloding listenig thread";

}
