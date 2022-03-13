
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;


double get_cpu_usage();
void get_hostname(char *response);
void get_cpu_name(char *response);


int main(int argc, char const *argv[]) {

    //looks for port number in command line arguments 
    int port_num;
    if (argc == 2)
    {
        istringstream iss( argv[1] );

        if (iss >> port_num)
        {
        }
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (sockfd == -1)
    {
        cout << "Failed to create a socket. errno: " << errno << endl;
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(int));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (const char *)&optval, sizeof(int));

    struct sockaddr_in address;
    int address_size = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port_num);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&address, address_size) < 0)
    {
        cout << "Failed to bind to port. errno: " << errno << endl;
        exit(EXIT_FAILURE);
    }

    
    if ((listen(sockfd, 5)) < 0)
    {
        cout << "Failed to listen on socket. errno: " << errno << endl;
        exit(EXIT_FAILURE);
    }

    char buff[1024];
    int comm_socket;

    while (true)
    {
    
        comm_socket = accept(sockfd, (struct  sockaddr*)&address, (socklen_t*)&address_size);

        if (comm_socket < 0)
            {
                cout << "Failed to grab connection. errno: " << errno << endl;
                exit(EXIT_FAILURE);
            }

        
        int res = 0;
        memset(buff, 0, sizeof(buff));
        
       
        res = read(comm_socket, buff, sizeof(buff));
        if (res < 0)
        {   
            cout << "Failed to read from client. errno: " << errno << endl;
            exit(EXIT_FAILURE);
        }
        
        char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";
        char result [1024];
        char response[2048];

        memset(result, 0, sizeof(result));
        memset(response, 0, sizeof(response));

        int function_return;
        double function_return_load;


        if (strncmp(buff, "GET /hostname", 13) == 0)
        {
            get_hostname(response);
            strcat(response, "");
        }
        else if (strncmp(buff, "GET /cpu-name", 13) == 0)
        {
            get_cpu_name(response);
            strcat(response, "");
        }
        else if (strncmp(buff, "GET /load", 9) == 0)
        {
            function_return_load = get_cpu_usage();
            sprintf(response, "%f", function_return_load);
            strcat(response, "%");
            strcat(response, "\n");
        }
        else
        {
            strcat(response, "400 BAD REQUEST\n");
        }
        
        strcat(header, response);

        send(comm_socket, header, strlen(header), 0);
        close(comm_socket);

    }

    return 0;
}



void get_hostname (char *response)
{
    char buff[1024];
    FILE *hostname = popen("cat /proc/sys/kernel/hostname | head -n 1", "r");
    fgets(buff, 1024, hostname);
    pclose(hostname);
    strcpy(response, buff);

}

void get_cpu_name (char *response) 
{
    char buff[1024];
    FILE *cpu_name = popen("cat /proc/cpuinfo | grep \"model name\" | head -n 1 | awk -F': ' '{print $2}'", "r");
    fgets(buff, 1024, cpu_name);
    pclose(cpu_name);
    strcpy(response, buff);
} 

class CpuParam {       
  public:             
    int NonIdle;        
    int Idle;  

    CpuParam calculate_Total_Idle_NonIdle()
    {
        int user;
        int nice;
        int system;
        int idle;
        int iowait;
        int irq;
        int softirq;
        int steal;

        CpuParam Total;
        if(ifstream("/proc/stat").ignore(3) >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal)
        {
            Total.Idle = idle + iowait;
            Total.NonIdle = user + nice + system + irq + softirq + steal;
        }
        return Total;
    }
};

double get_cpu_usage()
{
    CpuParam PrevTotal, CurrentTotal;

    PrevTotal.Idle = 0;
    PrevTotal.NonIdle = 0;
    CurrentTotal.Idle = 0;
    CurrentTotal.NonIdle = 0;

    int prevtotal;
    int currenttotal;
    int final_total;
    int idled;
    double result;

    PrevTotal = PrevTotal.calculate_Total_Idle_NonIdle();
    sleep(1);
    CurrentTotal = CurrentTotal.calculate_Total_Idle_NonIdle();

    prevtotal = PrevTotal.Idle + PrevTotal.NonIdle;
    currenttotal = CurrentTotal.Idle + CurrentTotal.NonIdle;

    final_total = currenttotal - prevtotal;
    idled = CurrentTotal.Idle - PrevTotal.Idle;
    result = (double(final_total - idled) / final_total)*100;

    return result;
}
