#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    std::string start_time = argv[1];

    int start_hour = atoi(start_time.substr(11, 2).c_str());
    int start_minute = atoi(start_time.substr(14, 2).c_str());
    int end_hour = 0;
    int end_minute = 0;

    if (start_minute >= 30)
    {
        end_hour = (start_hour + 1) % 24;
        end_minute = (start_minute + 30) % 60;
    }
    else
    {
        end_hour = start_hour;
        end_minute = start_minute + 30;
    }
    cout<<"************************************************************************************"<<endl;
    cout<<"Search time is from:\t"<<start_time<<" to:\t"<<start_time.substr(0,11)+to_string(end_hour)<<":"<< to_string(end_minute)<<endl;
    cout<<"************************************************************************************"<<endl;
    
    //the file that needs to get investigated to find recovery failure reason
    std::ifstream logFile("./algojob-algoserver_exec-1.log");
    // keyword we will search in a failed ADL
    std::string ADL = "ADL", adl = "adl", recovery = "recovery", Recovery = "Recovery", fail = "fail";
    bool is_parsing_required = true;
    std::string l;

    while (getline(logFile, l))
    {
        if (l.substr(0, 19) == start_time)
        {
            is_parsing_required = false; // no need to parse as we have find out the start time
            do
            {
                //check if it is a log line starting with date (2022 etc)
                if (l.substr(0, 3) == "202")
                {
                    int current_hour = atoi(l.substr(11, 2).c_str());
                    int current_minute = atoi(l.substr(14, 2).c_str());

                    if ((current_hour == end_hour) && (current_minute >= end_minute))
                        break;
                    //if some keywords matches, we will declare it as failed recovery
                    if (((l.find(ADL) != string::npos) || (l.find(adl) != string::npos)) && ((l.find(recovery) != string::npos) || (l.find(Recovery) != string::npos)) && (l.find(fail) != string::npos))
                        std::cout << l << std::endl;
                }
            }
            while(getline(logFile, l));
        }
        if (!is_parsing_required)
            break;
    }
    logFile.close();

    return 0;
}
