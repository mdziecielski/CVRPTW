#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

/*

1. Sort Customers by Due Date
2. Check validity - for every customer deploy one vehicle
    Check if

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

        void add_customer(Customer customer) {
            this->customers.push_back(customer);
        }

    private:
        int vehicle_number;
        int vehicle_capacity;
        std::vector<Customer> customers;

};

int main()
{
    //std::cout << "Hello GitHub!\n";

    std::ifstream example_input;
    example_input.open("./Input/cvrptw1.txt");
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
    int _;
    int x;
    int y;
    int dem;
    int ready;
    int due;
    int service;




    while (std::getline(example_input, line)) {
        std::cout << line << std::endl;
        if (line == ""){
            break;
        }
        std::stringstream s(line);
        s >> _ >> x >> y >> dem >> ready >> due >> service;
        problem.add_customer(Customer(x,y,dem,ready,due,service));
    }

    example_input.close();

    return 0;
}
