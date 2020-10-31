#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

/*

1. Sort Customers by Due Date 
2. Check validity - for every customer deploy one vehicle [X]
3. 
*/

class Customer {
    public:
        Customer(int X, int Y, int DEMAND, int READY_TIME, int END_TIME, int SERVICE_TIME) {
            this->x = X;
            this->y = Y;
            this->demand = DEMAND;
            this->ready_time = READY_TIME;
            this->end_time = END_TIME;
            this->service_time = SERVICE_TIME;
        };
        Customer(){};
        
        double get_distance(Customer pierwszy){
            return sqrt(pow(pierwszy.get_x() - this->x,2)+pow(pierwszy.get_y() - this->y,2));
        }

        int get_x() {
            return this->x;
        }

        int get_y() {
            return this->y;
        }

        int get_demand() {
            return this->demand;
        }

        int get_ready_time() {
            return this->ready_time;
        }

        int get_end_time() {
            return this->end_time;
        }

        int get_service_time() {
            return this->service_time;
        }

    private:
        int x;
        int y;
        int demand;
        int ready_time;
        int end_time;
        int service_time;
};

class CVRPTW {
    public:
        CVRPTW(int VEHICLE_NUMBER, int VEHICLE_CAPACITY) {
            this->vehicle_number = VEHICLE_NUMBER;
            this->vehicle_capacity = VEHICLE_CAPACITY;
        };
        void add_depo(Customer customer){
            this->depo.push_back(customer);
        }
        void add_customer(Customer customer) {
            this->customers.push_back(customer);
        }
        int check_validity(){
            for(auto x: customers){
                if ((x.get_x()==depo[0].get_x()) && (x.get_y()==this->depo[0].get_y())){
                    continue;
                }
                if(x.get_distance(this->depo[0]) >= x.get_ready_time()){
                    if(x.get_distance(this->depo[0])*2 + x.get_service_time() > this->depo[0].get_end_time()){
                        return -1;
                    }
                }
                else{
                    if(x.get_ready_time()+x.get_service_time()+x.get_distance(this->depo[0]) > this->depo[0].get_end_time()){
                        return -1;
                    }
                }


            }
            return 1;
        }

    private:
        int vehicle_number;
        int vehicle_capacity;
        std::vector<Customer> customers;
        std::vector<Customer> depo;
        
};

int main()
{
    //std::cout << "Hello GitHub!\n";

    std::ifstream example_input;
    example_input.open("./Input/rc21010.txt");
    std::string line;

    //pomijanie niewaznych linijek i zczytanie wlasnosci samochodu
    for(int i = 0; i < 5; i++)
    {
        std::getline(example_input, line);
    }

    std::stringstream s(line);
    int vehicle_n, vehicle_c;
    s >> vehicle_n >> vehicle_c;
    CVRPTW problem =  CVRPTW(vehicle_n, vehicle_c);


    //pomijanie niewaznych linijek
    for(int i = 0; i < 4; i++)
    {
        std::getline(example_input, line);
    }
    int customer_num;
    int x;
    int y;
    int dem;
    int ready;
    int due;
    int service;



    while (std::getline(example_input, line)) {
        if (line == ""){
            break;
        }
        std::stringstream s(line);
        s >> customer_num >> x >> y >> dem >> ready >> due >> service;
        if(customer_num == 0){
            problem.add_depo(Customer(x,y,dem,ready,due,service));
        }
        else{
        problem.add_customer(Customer(x,y,dem,ready,due,service));
        }
    }
    std::cout<<problem.check_validity();
    example_input.close();

    return 0;
}
