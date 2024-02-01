/***********************************************************************

Matr.-Nr:                     3169352
Nachname/Surname:             Elgali
Vorname/Given name:           Abdalla
Uni-Email:                    abdalla.elgali@stud.uni-due.de
Studiengang/Course of studis: B.Sc. Computer Engineering ISE (Software Engineering)

***********************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;


// Average consumption is in kWh
int avg_cons_person = 200;
int avg_cons_sqm = 9;
int avg_water_heat = 550;

const int width = 35;

class Device {
private:
    string description;
    Device* next;
public:
    Device(Device* pointer) : next(pointer) {
        next = nullptr;
    }
    Device() {}

    string get_description() {
        return description;
    }

    Device* get_next() {
        return next;
    }
    void set_description(string desc) {
        description = desc;
    }
    void set_next(Device* nxt) {
        next = nxt;
    }
    virtual void input() {
        string desc;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "What is the description: ";
        getline(cin, desc);
        set_description(desc);

    }

    virtual void print(int num, float num_2) = 0;

    virtual float annual_kWh() = 0;

    virtual Device* get_a_copy() = 0;

    virtual string get_string_for_file(char c) = 0;


    virtual ~Device() { cout << "Device at " << this << " is deconstructed\n\n"; };
};


class Consumer : public Device
{

protected:
    enum freq_of_use { once, daily, mo_fr, sa_su, weekly };

private:
    float watt;
    freq_of_use use;
    int static total_consumers;

public:
    //Default constructor
    Consumer(Device* pointer = nullptr) : Device(pointer), watt(0.00), use() {
        total_consumers++;
    }



    float get_watt() {
        return watt;
    }

    freq_of_use get_use() {
        return use;
    }
    string get_use(freq_of_use use_to_string) {
        switch (use_to_string) {
        case once:
            return "Once";
        case daily:
            return "Daily";
        case mo_fr:
            return "Monday to Friday";
        case sa_su:
            return "Saturday and Sunday";
        case weekly:
            return "Weekly";
        }
    }

    int get_total_consumers() {
        return total_consumers;
    }

    void set_watt(float w) {
        this->watt = w;
    }

    void set_use(freq_of_use usage) {
        this->use = usage;
    }

    freq_of_use input_use() {
        char opt;
        cout << "How often is it used?\n once (o)\n daily (d)\n mo_fr (m)\n sa_su (s)\n weekly (w)? ";
        cin >> opt;

        switch (opt) {
        case 'o':
            return once;
        case 'd':
            return daily;
        case 'm':
            return mo_fr;
        case 's':
            return sa_su;
        case 'w':
            return weekly;
        default:
            cout << "Choose one of the given options and try again";
            return input_use();
        }
    }

    freq_of_use find_use(string f_u) {

        freq_of_use use;

        if (f_u == "daily") {
            use = daily;
        }
        else if (f_u == "once") {
            use = once;
        }
        else if (f_u == "Monday to Friday") {
            use = mo_fr;

        }
        else if (f_u == "Saturday and Sunday") {
            use = sa_su;
        }
        else if (f_u == "weekly") {
            use = weekly;
        }
        return use;

    }
    void input() override {

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Device::input();

        float w;
        cout << "How many watts does it consume? ";
        cin >> w;
        set_watt(w);

        freq_of_use u;
        u = input_use();
        set_use(u);

    }


    ~Consumer() {
        total_consumers--;
        cout << "Consumer at address " << this << " deleted " << endl;
    }
};

int Consumer::total_consumers = 0;  // has to be static in order to be initialized outside the class

class Immobile : public Consumer
{

private:
    float hours;
    float watt_standby;

public:
    Immobile(Device* pointer = nullptr) : Consumer(pointer) {};

    float get_watt_standby() {
        return watt_standby;
    }
    float get_hours() {
        return hours;
    }

    void set_watt_standby(float w_s) {
        this->watt_standby = w_s;

    }
    void set_hours(float hour) {
        this->hours = hour;
    }
    void input() override {

    }

    float annual_hours_of_use() {
        switch (get_use()) {
        case once:
            return hours;
        case daily:
            return hours * 365;
        case mo_fr:
            return hours * 260;
        case sa_su:
            return hours * 104;
        case weekly:
            return hours * 52;
        default:
            return 0;
        }
    }
    float annual_hours_of_standby() {
        float standby = 8760 - annual_hours_of_use();
        return standby;
    }
    float annual_kWh() override {
        float total_hours = annual_hours_of_use() + annual_hours_of_standby();
        float consumption = (get_watt() * annual_hours_of_use() / 1000) + (watt_standby * annual_hours_of_standby() / 1000);

        return consumption;
    }
    virtual Device* get_a_copy() override {
        Immobile* copy = new Immobile(*this);
        return copy;
    }
    string to_string_with_precision(float value, int precision) {
        ostringstream ss;
        ss << fixed << setprecision(precision) << value;
        return ss.str();
    }

    virtual string get_string_for_file(char c) override {
        string result = "Immobile";
        string watt = to_string_with_precision(get_watt(), 2);
        string standby = to_string_with_precision(get_watt_standby(), 2);
        string hours = to_string_with_precision(get_hours(), 2);
        string annual_hours = to_string_with_precision(annual_hours_of_use(), 2);
        string annual_hours_standby = to_string_with_precision(annual_hours_of_standby(), 2);
        string annual_kwh = to_string_with_precision(annual_kWh(), 2);
        result.push_back(c);
        result += get_description() + c + watt + c +
            standby + c + hours + c + annual_hours + c + annual_hours_standby + c +
            annual_kwh;

        return result;
    }


    virtual void print(int num, float price_kWh) override {


        cout << right << setw(width) << num << ": " << get_description() << "  (at address: " << this << ") \n";
        cout << right << setw(width) << "power consumption: " << left << fixed << setprecision(2) << get_watt() << " W\n";
        cout << right << setw(width) << "power consumption standby: " << left << fixed << setprecision(2) << watt_standby << " W\n";
        cout << right << setw(width) << "annual hours of use: " << left << fixed << setprecision(2) << annual_hours_of_use() << " h\n";
        cout << right << setw(width) << "annual hours of standby: " << left << fixed << setprecision(2) << annual_hours_of_standby() << " h\n";
        cout << right << setw(width) << "annual consumption: " << left << fixed << setprecision(1) << annual_kWh() << " kWh\n";
        cout << right << setw(width) << "annual costs: " << left << fixed << setprecision(2) << annual_kWh() * price_kWh << " EUR\n";
    }
    virtual ~Immobile() { cout << "Immobile at " << this << " deconstructed" << endl; }
};

class Mobile : public Consumer
{
private:
    float km;


public:
    Mobile(Device* pointer = nullptr) : Consumer(pointer), km(0.00) {}

    float get_km() {
        return km;
    }
    void set_km(float kms) {
        km = kms;
    }
    void input() override {
        float kms;
        Consumer::input();
        cout << "Kms: ";
        cin >> kms;
        set_km(kms);
    }


    float annual_hours_of_use() {


        switch (get_use()) {
        case once:
            return 1;
        case daily:
            return 365;
        case mo_fr:
            return 260;
        case sa_su:
            return 104;
        case weekly:
            return 52;
        default:
            return 0;
        }
    }

    float annual_kWh() override {

        float total_hours = annual_hours_of_use();

        float consumption_per_100km = (get_watt() / 1000) / 100;  // kW per 100 km

        float consumption = (km)*consumption_per_100km;

        float annual_cons = consumption * total_hours;
        return annual_cons;
    }

    virtual void print(int num, float price_kWh) override {
        string use = get_use(get_use());

        cout << right << setw(width) << num << ": " << get_description() << "  (at address: " << this << ") \n";
        cout << right << setw(width) << "power consumption per 100 km: " << left << fixed << setprecision(2) << get_watt() / 1000 << " kW\n";
        cout << right << setw(width) << "Km Driven: " << left << fixed << setprecision(2) << get_km() << " km " << use << "\n";
        cout << right << setw(width) << "annual consumption: " << left << fixed << setprecision(1) << annual_kWh() << " kWh\n";
        cout << right << setw(width) << "annual costs: " << left << fixed << setprecision(2) << annual_kWh() * price_kWh << " EUR\n";
    }

    Device* get_a_copy() override {
        Mobile* copy = new Mobile(*this);
        return copy;
    }
    virtual string get_string_for_file(char c) override {
        string result = "Mobile";
        result.push_back(c);
        result += get_description() + c + to_string(get_watt()) + c +
            to_string(get_km()) + c + to_string(annual_hours_of_use()) + c + to_string(annual_kWh());
        return result;
    }

    virtual ~Mobile() { cout << "Mobile at " << this << " deconstructed" << endl; }

};


class Producer : public Device
{
public:
    Producer(Device* pointer = nullptr) {};

    void input() {
        Device::input();
    }
    virtual ~Producer() { cout << "Producer at " << this << " deconstructed" << endl; }
};

class Solar : public Producer {
private:
    int watt_peak;
    int year;
public:
    Solar(Device* pointer = nullptr) {};

    int get_watt_peak() {
        return watt_peak;
    }
    int get_year() {
        return year;
    }
    void set_watt_peak(int w_p) {
        this->watt_peak = w_p;
    }
    void set_year(int yr) {
        this->year = yr;
    }
    void input() override {
        Device::input();

        int yr;
        int watt_peak;

        cout << "Watt peak: ";
        cin >> watt_peak;
        set_watt_peak(watt_peak);

        cout << "Year: ";
        cin >> yr;
        set_year(yr);
    }

    virtual float annual_kWh() override {

        float decrease_per_yr = .005;
        float total_decrease_decimal = (2024 - year) * decrease_per_yr;

        return -(watt_peak - (watt_peak * total_decrease_decimal));
    }
    virtual void print(int num, float price_kWh) override {

        cout << right << setw(width) << num << ": " << this->get_description() << "  (at address: " << this << ") \n";
        cout << right << setw(width) << "solar cells installed power: " << left << fixed << setprecision(2) << watt_peak << " watt_peak\n";
        cout << right << setw(width) << "age of solar cells: " << left << fixed << setprecision(2) << (2024 - year) << " years\n";
        cout << right << setw(width) << "annual production: " << left << fixed << setprecision(1) << annual_kWh() << " kWh\n";
        cout << right << setw(width) << "annual savings: " << left << fixed << setprecision(2) << annual_kWh() * price_kWh << " EUR\n";
    }

    Device* get_a_copy() override {
        Solar* copy = new Solar(*this);
        return copy;
    }
    virtual string get_string_for_file(char c) override {
        string result = "Solar";
        result.push_back(c);
        result += get_description() + c + to_string(get_watt_peak()) + c +
            to_string(get_year()) + c + to_string(annual_kWh());
        return result;
    }
};

class Household {
private:
    float price_kWh;
    string power_supplier;
    int persons;
    float square_meters;
    bool electric_water_heating;
    Device* head = nullptr; // represents the head of the consumers list, it represents the connection between the household and consumer class in the class diagram
public:
    Household(float price_kWh, string power_supplier, int persons, float square_meters, bool electric_water_heating) : price_kWh(price_kWh), power_supplier(power_supplier), persons(persons), square_meters(square_meters), electric_water_heating(electric_water_heating) {
        head = nullptr;
    }

    float get_price_kWh() {
        return price_kWh;
    }
    string get_power_supplier() {
        return power_supplier;
    }
    int get_persons() {
        return persons;
    }
    float get_square_meters() {
        return square_meters;
    }
    bool get_electric_water_heating() {
        return electric_water_heating;
    }
    Device* get_devices() {
        return this->head;
    }
    void set_price_kWh(float power_price) {
        price_kWh = power_price;
    }
    void set_power_supplier(string supplier) {
        power_supplier = supplier;
    }
    void set_persons(int person) {
        persons = person;
    }
    void set_square_meters(float meters) {
        square_meters = meters;
    }
    void set_electric_water_heating(bool heating) {
        electric_water_heating = heating;
    }
    void set_consumers(Device* consumer) {
        this->head = consumer;
    }
    void add_consumers_to_household(Device* new_device) {  // FILO: First in last out
        Device* current_head = get_devices();
        Device* current_next = new_device->get_next();
        new_device->set_next(current_head);

        Device* new_head = new_device;
        set_consumers(new_head);

    }
    void operator+(Device* new_device) { // LILO: Last in last out: Used by 

        Device* current_head = get_devices(); // headptr

        if (current_head == nullptr) {
            set_consumers(new_device);  // set the new_device as head
        }
        else {
            while (current_head->get_next() != nullptr) {
                current_head = current_head->get_next();
            }
            current_head->set_next(new_device);
            
       }
    }


    Device* move_up(int k) {
        Device* head = get_devices();
        if (head == nullptr || k <= 1) {
            return head;
        }

        Device* current = head;
        Device* prev = nullptr;
        int currentPosition = 1;

        while ((current != nullptr) && (currentPosition < k)) {
            prev = current;
            current = current->get_next();
            currentPosition++;
        }

        if (k == 2) {
            
            prev->set_next(current->get_next());
            current->set_next(prev);
            head = current;
            set_consumers(head);
        }
        else {
            Device* curr = get_devices();
            while (curr->get_next() != prev) {
                curr = curr->get_next();
            }

            if (current->get_next() == nullptr) {
                prev->set_next(nullptr);
            }
            else {
                prev->set_next(current->get_next());
            }
            curr->set_next(current);
            current->set_next(prev);

        }   


        return head;
    }

    Device* reverse_list(Device* head) {
        Device* prev = nullptr;
        Device* current = head;
        Device* next = nullptr;

        while (current != nullptr) {
            next = current->get_next();
            current->set_next(prev);
            prev = current;
            current = next;
        }
        return prev;
    }

    void copy_devices(Household* household_to_copy_from) {
        Device* current = household_to_copy_from->get_devices();
        Device* new_head = nullptr; // This will be the head of the new list of devices

        if (this->get_devices() == nullptr) {
            while (current != nullptr) {

                // Create a copy of the current device
                Device* copied_device = current->get_a_copy();

                copied_device->set_next(new_head);
                new_head = copied_device;
                current = current->get_next();


            }

            Device* reversed_list = reverse_list(new_head);
            this->set_consumers(reversed_list);
        }
        else {
            while (current != nullptr) {

                // Create a copy of the current device
                Device* copied_device = current->get_a_copy();

                copied_device->set_next(new_head);
                new_head = copied_device;
                current = current->get_next();


            }
            Device* reversed_list = reverse_list(new_head);
            Device* current = reversed_list;
            while (current->get_next() != nullptr) { current = current->get_next(); }
            current->set_next(get_devices());
            set_consumers(reversed_list);

        }
        }
        
    void input() {

        cout << "Enter number of persons: ";
        cin >> persons;
        set_persons(persons);

        cout << "Enter square meters: ";
        cin >> square_meters;
        set_square_meters(square_meters);

        cout << "Electric water heating (1 for true, 0 for false): ";
        cin >> electric_water_heating;
        set_electric_water_heating(electric_water_heating);

        cout << "Enter price per kWh: ";
        cin >> price_kWh;
        set_price_kWh(price_kWh);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter power supplier: ";
        getline(cin, power_supplier);
        set_power_supplier(power_supplier);


    }
    void print(int household_number, string city, Household* household) {

        cout << "\nH O U S H E H O L D  N O.  " << household_number << "  P O W E R   C O N S U M P T I O N\n";
        string bol;
        float power_cons = avg_cons_sqm * square_meters;
        float power_cons_p;
        cout << left << setw(width) << "----------------------------------------------------------------------" << "\n";
        if (electric_water_heating == true) {
            bol = "yes";
            power_cons_p = avg_water_heat * persons;
        }
        else {
            bol = "no";
            power_cons_p = avg_cons_person * persons;
        }

        cout << left << setw(width) << "city: " << right << city << "  (at address: " << household << ") \n";
        cout << left << setw(width) << fixed << setprecision(2) << "price for one kWh: " << right << price_kWh * 100 << " ct/kWh\n";
        cout << left << setw(width) << "power supplier: " << right << power_supplier << "\n";
        cout << left << setw(width) << "square meters: " << right << square_meters << " qm\n";
        cout << left << setw(width) << "persons: " << right << persons << "\n";
        cout << left << setw(width) << "water heated using electricity: " << right << bol << "\n";
        cout << left << setw(width) << "list of consumers\n" << endl;
        cout << setw(2 * width) << "----------------------------------------------------------------------" << "\n";


        Device* current_consumer = get_devices(); // head ptr
        float total_cons = power_cons + power_cons_p;
        float total_costs = total_cons * price_kWh; // power_price is in EUR
        int p = 1;
        while (current_consumer != nullptr) {
            if (p > 10) {
                break;
            }
            current_consumer->print(p, price_kWh);
            float annual_kwh = current_consumer->annual_kWh();
            total_cons += annual_kwh;
            total_costs += annual_kwh * price_kWh;
            current_consumer = current_consumer->get_next();
            p++;

        }




        cout << left << setw(width) << "----------------------------------------------------------------------" << "\n";
        cout << left << setw(width) << "power consumption square meters: " << right << fixed << setprecision(1) << power_cons << " kWh\n";
        cout << left << setw(width) << "power consumption all persons: " << right << fixed << setprecision(1) << power_cons_p << " kWh\n";
        cout << left << setw(width) << "total annual power consumption: " << right << fixed << setprecision(1) << total_cons << " kWh\n";
        cout << left << setw(width) << "total annual power costs: " << right << fixed << setprecision(1) << total_costs << " EUR\n\n";

    }

    ~Household() {
        Device* current = get_devices();
        Device* to_be_deleted;

        while (current != nullptr) {
            to_be_deleted = current;

            Device* next = current->get_next();

            current->set_next(nullptr);

            delete to_be_deleted;

            current = next;
        }
        set_consumers(nullptr);
        cout << "Household at address " << this << " deleted " << endl;
    }

};



class Address {
private:

    string street;
    string no;
    string zip;
    string city;

public:
    // Parametrized constructor
    Address(string street, string no, string zip, string city) : street(street), no(no), zip(zip), city(city) {}

    // Default constructor
    Address() : street(""), no(""), zip(""), city("") {}

    string get_street() {
        return street;
    }
    string get_no() {
        return no;
    }
    string get_zip() {
        return zip;
    }
    string get_city() {
        return city;
    }
    string to_string() {
        string myAddress;
        myAddress.append(street).append(" ").append(no).append(" ").append(zip).append(" ").append(city);
        return myAddress;
    }
    void input() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter street: ";
        getline(cin, street);

        cout << "Enter street number: ";
        getline(cin, no);

        cout << "Enter zip code: ";
        getline(cin, zip);

        cout << "Enter city: ";
        getline(cin, city);
    }

    ~Address() {
        string addr = to_string();
        cout << "Address " << addr << " at address " << this << " deleted " << endl;
    }
};


class House {
private:
    Household** household;
    int number_of_households;
    Address address;
public:
    // Parametrized constructor
    House(int n_households, Address& addres) : number_of_households(n_households), address(addres) {
        household = new Household * [number_of_households];
        for (int i = 0; i < number_of_households; ++i) {
            household[i] = nullptr;
        }
    }


    Household* get_household(int household_number) {
        int real_number_of_household = household_number - 1;
        if (real_number_of_household >= 0 && real_number_of_household < number_of_households) {
            return household[real_number_of_household];
        }
        else {
            return nullptr;
        }
    }


    Household* operator[](int household_number) {  // Literally the exact same method as get_household

        int real_number_of_household = household_number - 1;
        if (real_number_of_household >= 0 && real_number_of_household < number_of_households) {
            return household[real_number_of_household];
        }
        else {
            return nullptr;
        }
    }
    void set_household(int number_of_household) {

        int real_number_of_household = number_of_household;

        Household* this_household = get_household(real_number_of_household);

        if (this_household != nullptr) {
            cout << "Household already exists! " << endl;

        }
        else if (real_number_of_household > number_of_households) {
            cout << "Invalid number of Household, the available number of households in this House: " << number_of_households << endl;
        }
        else {

            Household* new_household = new Household(0, "", 0, 0.0, false);
            new_household->input();
            household[real_number_of_household-1] = new_household;
        }


    }
    void print_all() {
        Consumer* fake_consumer = new Immobile();
        int total_consumers = fake_consumer->get_total_consumers() - 1;

        delete fake_consumer;

        cout << left << setw(width) << "----------------------------------------------------------------------" << "\n";
        cout << left << setw(width) << " H O U S E " << "\n";
        cout << left << setw(width) << "----------------------------------------------------------------------" << "\n";
        cout << left << setw(width) << "(this: " << this << ")\n";
        cout << left << setw(width) << "address: " << address.to_string() << "\n";
        cout << left << setw(width) << "number of households: " << number_of_households << "\n";
        cout << left << setw(width) << "total number of consumers: " << total_consumers << "\n";
        for (int i = 0; i < number_of_households; i++) {

            if (get_household(i) != nullptr) {
                get_household(i)->print(i, address.get_city(), get_household(i)); // n+1 as the index starts from 0, while for user it starts from 1
            }
        }
    }
    void write_to_file(string file_name, char delim) {

        ofstream file;
        file.open(file_name, ios::app);
        cout << "\n Writing file '" << file_name << "'... " << endl;
        string water;
        file << "A6" << delim << number_of_households << delim << address.get_street() << delim << address.get_no() << delim << address.get_zip() << delim << address.get_city() << "\n";  // price by 100 as it is passed in in cents rather than euros

        for (int h = 0; h < number_of_households; h++) {
            if (household[h] != nullptr) {

                if (household[h]->get_electric_water_heating()) {
                    water = "true";
                }
                else {
                    water = "false";
                }

                file << "household" << delim << h + 1 << delim << water << delim << household[h]->get_persons() << delim << household[h]->get_square_meters() << delim << household[h]->get_price_kWh() << delim << household[h]->get_power_supplier() << "\n";
                Device* current_consumer = household[h]->get_devices();
                while (current_consumer != nullptr) {
                    string use;
                    file << current_consumer->get_string_for_file(delim) << "\n";
                    current_consumer = current_consumer->get_next();
                }
            }
        }
        cout << " Closing file '" << file_name << "'... " << endl;
        file.close();
    }



    ~House() {
        for (int i = 0; i < number_of_households; i++) {
            if (household[i] != nullptr) {
                delete household[i];
            }
        }
        cout << "House at address " << this << " deleted" << endl;
    }
    static void read_from_file(string file_name, char delim, House*& new_house);
};

void House::read_from_file(string file_name, char delim, House*& new_house) {

    ifstream file(file_name); // open file in read
    cout << "\nReading file '" << file_name << "'... " << endl;

    Address new_address = Address();
    //House* new_house = new House(0, *new_address);

    string line;
    string s;
    int count = 0;
    int household_number = -1;

    string city;
    string street;
    string street_no;
    string zip;

    float price;

    while (getline(file, line)) {
        stringstream inputString(line);

        if (count == 0) { // To parse out the first line of each file 
            string task;
            int n_of_households = 0;

            getline(inputString, task, delim);

            string temp_string = "";
            getline(inputString, temp_string, delim);
            n_of_households = atoi(temp_string.c_str());

            getline(inputString, street, delim);
            street = street;

            getline(inputString, street_no, delim);
            street_no = street_no;

            getline(inputString, zip, delim);
            zip = zip;

            getline(inputString, city, delim);
            city = city;

            new_address = Address(street, street_no, zip, city);
            new_house = new House(n_of_households, new_address); // initialize the house // As per the second example run

        }
        else {
            string temp;

            int n_of_household;
            int p_of_household;
            string temp_string = "";
            string hot_water;
            bool water;
            string city;
            int sqm;
            float price_kwh;
            string power_sup;

            string consumer;
            string type;
            float kms;
            string f_use;
            float watt;
            float standby;
            float hours;

            int year;

            getline(inputString, temp, delim); // 1

            if (temp == "Household")
            {
                getline(inputString, temp_string, delim); // 2
                n_of_household = atoi(temp_string.c_str());

                getline(inputString, hot_water, delim); // 3

                if (hot_water == "true") {
                    water = true;
                }
                else {
                    water = false;
                }

                temp_string = "";
                getline(inputString, temp_string, delim); // 4 
                p_of_household = atoi(temp_string.c_str());

                temp_string = "";
                getline(inputString, temp_string, delim); // 5
                sqm = atoi(temp_string.c_str());

                temp_string = ""; // The price of the household
                getline(inputString, temp_string, delim); // 6
                price_kwh = atof(temp_string.c_str());
                price = price_kwh / 100;

                getline(inputString, power_sup, delim); // 7


                // Input the household, append it in the households array
                Household* new_household = new Household(price_kwh, power_sup, p_of_household, sqm, water);
                new_house->household[n_of_household - 1] = new_household;
                household_number = n_of_household - 1;
            }
            else if (temp == "Mobile") {

                Mobile* new_consumer = new Mobile();

                getline(inputString, type, delim); // 2

                temp_string = "";
                getline(inputString, temp_string, delim); // 3
                kms = atof(temp_string.c_str());

                getline(inputString, f_use, delim); // 4


                temp_string = "";
                getline(inputString, temp_string, delim); // 5
                watt = atof(temp_string.c_str());


                // Input the consumer per the household specified. HINT: consumers followed by a household belong to said household
                new_consumer->set_description(type);
                new_consumer->set_km(kms);
                new_consumer->set_use(new_consumer->find_use(f_use));
                new_consumer->set_watt(watt);
                Household* this_household = new_house->household[household_number];

                *this_household + new_consumer;
                cout << "Adding cons to household!! MOBILE " << endl;
            }
            else if (temp == "Immobile") {
                cout << "YOOOOO NOT SOLAARRR";
                Immobile* new_consumer = new Immobile();

                getline(inputString, type, delim); // 2

                temp_string = "";
                getline(inputString, temp_string, delim); // 3
                hours = atof(temp_string.c_str());

                getline(inputString, f_use, delim); // 4


                temp_string = "";
                getline(inputString, temp_string, delim); // 5
                watt = atof(temp_string.c_str());

                temp_string = "";
                getline(inputString, temp_string, delim); // 6
                standby = atof(temp_string.c_str());

                // Input the consumer per the household specified. HINT: consumers followed by a household belong to said household
                new_consumer->set_description(type);
                new_consumer->set_hours(hours);
                new_consumer->set_use(new_consumer->find_use(f_use));
                new_consumer->set_watt(watt);
                new_consumer->set_watt_standby(standby);

                Household* this_household = new_house->household[household_number];

                *this_household + new_consumer;

                cout << "Adding cons to household!! IMMOBILE " << endl;

                //new_house->household[household_number]->add_consumers_to_household(new_consumer);
            }
            else if (temp == "Solar") {

                Solar* new_consumer = new Solar();

                getline(inputString, type, delim); // 2

                temp_string = "";
                getline(inputString, temp_string, delim); // 3
                year = atoi(temp_string.c_str());


                temp_string = "";
                getline(inputString, temp_string, delim); // 5
                watt = atof(temp_string.c_str());



                // Input the consumer per the household specified. HINT: consumers followed by a household belong to said household
                new_consumer->set_description(type);
                new_consumer->set_year(year);
                new_consumer->set_watt_peak(watt);

                Household* this_household = new_house->household[household_number];

                *this_household + new_consumer;
                cout << "Adding cons to household!! SOLAR " << endl;
            }

        }

        count++;
    }


    cout << "Closing file '" << file_name << "'... " << endl;
    file.close();
}

int main()
{

    int number_of_households;
    Address myAddress;
    bool initialized = false;
    House* myHouse = nullptr;


    cout << " CALCULATION OF AVERAGE POWER COSTS FOR A HOUSEHOLD! - Class Version \n" << endl;

    // Clear the input buffer
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');


    char opt = 's';
    while (opt != 'q') {

        cout << "\n q quit\n h house initialization \n r read file\n w write file\n i input immobile consumer\n m input mobile consumer \n s input solar producer \n u move up power consumer\n p print household\n a print all households\n n new households\n c copy all consumers (added to already existing ones)\n d delete house\n >> ";
        cin >> opt;

        if (opt == 'p') { // print household
            int number_of_household;
            cout << "Which household: ";
            cin >> number_of_household;

            myHouse->get_household(number_of_household)->print(number_of_household, myAddress.get_city(), myHouse->get_household(number_of_household));
        }


        else if (opt == 'h') { // initialize the House
            cout << "How many houesholds does the house have? ";
            cin >> number_of_households;
            myAddress.input();
            myHouse = new House(number_of_households, myAddress); // initialize the house
            initialized = true;
        }


        else if (opt == 'r') { // read from file
            string file_name;
            char delim;
            cout << "File to read from: ";
            cin >> file_name;
            cout << "Delim: ";
            cin >> delim;
            House::read_from_file(file_name, delim, myHouse);
            initialized = true;
        }


        else if (opt == 'w') { // write to file
            string file_name;
            cout << "File name: ";
            cin >> file_name;
            cout << "Delim: ";
            char delim;
            cin >> delim;

            myHouse->write_to_file(file_name, delim);
        }


        else if (opt == 'i') {  // input immobile power consumer
            int number_of_household;
            cout << "Household number: ";
            cin >> number_of_household;

            if (myHouse->get_household(number_of_household) == nullptr) {
                cout << "Household does not exist!";
            }
            else {
                Immobile* myConsumer = new Immobile();
                myConsumer->input();
                Household* household_ptr = myHouse->get_household(number_of_household);
                household_ptr->add_consumers_to_household(myConsumer);
            }
        }
        else if (opt == 'm') {  // input mobile consumer
            int number_of_household;
            cout << "Household number: ";
            cin >> number_of_household;

            if (myHouse->get_household(number_of_household) == nullptr) {
                cout << "Household does not exist!";
            }
            else {
                Mobile* myConsumer = new Mobile();
                myConsumer->input();
                Household* household_ptr = myHouse->get_household(number_of_household);
                household_ptr->add_consumers_to_household(myConsumer);
            }
        }
        else if (opt == 's') {  // input solar producer
            int number_of_household;
            cout << "Household number: ";
            cin >> number_of_household;

            if (myHouse->get_household(number_of_household) == nullptr) {
                cout << "Household does not exist!";
            }
            else {
                Solar* myConsumer = new Solar();
                myConsumer->input();
                Household* household_ptr = myHouse->get_household(number_of_household);
                household_ptr->add_consumers_to_household(myConsumer);
            }
        }
        else if (opt == 'u') { // move up a consumer
            int number_of_household;
            int number_of_consumer;
            cout << "Which houeshold: ";
            cin >> number_of_household;
            cout << "Which consumer: ";
            cin >> number_of_consumer;

            myHouse->get_household(number_of_household)->move_up(number_of_consumer);
        }
        else if (opt == 'n') { // add new household
            int number_of_household;

            if (!initialized) {
                cout << "house is a nullptr, please first choose h to initialise a new house" << endl;
            }
            else {
                cout << "Number of household: ";
                cin >> number_of_household;
                myHouse->set_household(number_of_household);
            }
        }
        else if (opt == 'c') { // copy consumers
            int number_of_houeshold_1;
            int number_of_houeshold_2;

            cout << "Household to copy from: ";
            cin >> number_of_houeshold_1;

            Household* household_1 = myHouse->get_household(number_of_houeshold_1);

            cout << "To which household: ";
            cin >> number_of_houeshold_2;

            //Household* TempHousehold = new Household(household_1->get_price_kWh(), household_1->get_power_supplier(), household_1->get_persons(), household_1->get_square_meters(), household_1->get_electric_water_heating());

            myHouse->get_household(number_of_houeshold_2)->copy_devices(household_1);
        }
        else if (opt == 'a') { // print all
            if (initialized == false) {

                cout << "house is a nullptr, please first choose h to initialise a new house" << endl;
            }
            else {
                myHouse->print_all();
            }
        }
        else if (opt == 'd') {
            if (initialized) {
                delete myHouse;
                delete& myAddress;
            }
            else {
                cout << "house is nullptr, please first choose h to initialize a new house" << endl;
            }
        }
        else if (opt == 'q') { // quit
            break;
        }
        else {
            cout << "Invalid Character, retry again!\n";
        }

    }
    return 0;
}

// ORDER of which the consumers are copied/ appended and written has to be checked and changed,, In the copy_consumers method

/*
1)  g++ -std=c++20 -o a6_3169352 a6_3169352.cpp
2) ./a6_3169352
*/