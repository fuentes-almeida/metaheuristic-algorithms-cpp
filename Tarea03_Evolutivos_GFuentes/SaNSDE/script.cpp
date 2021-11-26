#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>

using namespace std;

int main()
{
    vector<string> machines;                    ///vector para guardar el archivo de maquinas
    string directory;                           ///para direccion del ejecutable y parametros
    ifstream task("G_Fuentes/SaNSDE/taskfile.txt");
    ifstream mach("G_Fuentes/SaNSDE/machinefile.txt");

    while (!mach.eof())                         ///lectura del archivo de maquinas
    {
        if (!getline(mach,directory))
            break;
        machines.push_back(directory);
    }

    int num_children=machines.size();
    pid_t *childrenPIDs=(pid_t*)malloc(num_children*sizeof(pid_t));
    pid_t process;

    while (!task.eof())
    {
        for (int i=0;i<num_children;i++)        ///una bifurcacion para cada maquina
        {
            string command;
            if(!getline(task,directory))
                break;
            command="ssh "+machines[i]+" "+directory;   ///comando completo
            process=fork();                             ///bifurcacion
            usleep(200000);
            if (process==0)                             ///proceso "hijo"
            {
                //cout<<getpid()<<" "<<command<<endl;
                system(&command[0]);            ///ejecucion del comando
                exit(0);
            }
            if (process<0)
            {
              cout<<"Error"<<endl;
            }
        }
    }

    int Waiting;             ///espera a que cada proceso hijo termine
        do {
           Waiting = 0;
            for (int i=0; i<num_children;i++)
            {
               if (childrenPIDs[i]>0)
               {
                  if (waitpid(childrenPIDs[i], NULL, WNOHANG) != 0)
                     childrenPIDs[i] = 0;       ///el hijo ha terminado
                  else
                     Waiting = 1;               ///sique en espera
               }
               sleep(0);
            }
        } while (Waiting);

    task.close();
    mach.close();
    free(childrenPIDs);

    return 0;
}
