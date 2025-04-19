#include <iostream>
using namespace std;

int main () {

    int choice;
    do{
        cout<<"**********Mini Wasilni**********"<<endl;
        cout <<"1-Add city"<<endl;
        cout <<"2-Add edge"<<endl;
        cout <<"3-Display graph"<<endl;
        cout <<"4-Traverse"<<endl;
        cout <<"5-Shortest path"<<endl;
        cout <<"6-Save"<<endl;
        cout <<"7-Load"<<endl;
        cout <<"8-exit"<<endl;
        cout <<"Enter your choice:"<<endl;
        
        cin>>choice;
        switch(choice){
            case 1:
            cout<<"1";
            break;

            case 2:
            cout<<"2";
            break;

            case 3:
            cout<<"3";
            break;

            case 4:
            cout<<"4";
            break;

            case 5:
            cout<<"5";
            break;

            case 6:
            cout<<"6";
            break;

            case 7:
            cout<<"7";
            break;

            default:
            cout<<"Invalid choice!!!"<<endl;
        }
    }while(choice!=8);
system("pause");
    return 0;
}