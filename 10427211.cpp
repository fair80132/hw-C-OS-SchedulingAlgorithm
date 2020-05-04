#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <cstdlib>

using namespace std ;

struct Process {

    int ID = 0 ;
    int CPUBurst = 0 ;
    int ArrivalTime = 0 ;
    int Prior = 0 ;
    int fcfs[2] = {0,0} ; // 0 waiting time, 1 turnaround time
    int nsjf[2] = {0,0} ;
    int psjf[2] = {0,0} ;
    int rr[4] = {0,0,0,0} ; // rr[2] => new arrive , [3] requene ;
    int pp[3] = {0,0,0} ;
    int leave = 0 ;

};

int gTimeSlice = 0 ;
int gMethod = 0 ;
string filename ;

void getInput( vector<Process> &list, string filename ) ;
void IDsort( vector<Process> &list ) ;
void FCFS( vector<Process> &list ) ;
void NSJF( vector<Process> &list ) ;
void PSJF( vector<Process> &list ) ;
void RR( vector<Process> &list ) ;
void PP( vector<Process> &list ) ;
void PrintResult( string filename, vector<Process> list ) ;
int arrivalSmallest( vector<Process> list, bool done[] ) ;
char IDtransform( int ID ) ;


int main() {

    vector<Process> input ;

    cout << "Enter filename(input.txt -> enter \"input\") :" ;
    cin >> filename ;
    filename = filename + ".txt" ;
    getInput( input, filename ) ;
    IDsort( input ) ;

    // cout << gMethod << gTimeSlice << endl ;
    // cout << input.at(6).ID << " " << input.at(6).CPUBurst << " " << input.at(6).ArrivalTime << " " << input.at(6).Prior << endl ;

    filename = filename + "_output.txt" ;
    if ( gMethod >= 1 )
        FCFS( input ) ;
    if ( gMethod >= 2 )
        RR( input ) ;
    if ( gMethod >= 3 )
        PSJF( input ) ;
    if ( gMethod >= 4 )
        NSJF( input ) ;
    if ( gMethod >= 5 )
        PP( input ) ;

    cout << "===========================================================" << endl ;
    PrintResult( filename, input ) ;

} // main()

void getInput( vector<Process> &list, string filename ) {

	fstream inFile ;
	char buffer[255] ;

	inFile.open( filename.c_str(), fstream::in ) ; // open file ;

    // Get Method & TimeSlice *************************************************************************************** //
	inFile.getline( buffer, 255, '\n' ) ;
	string strcontrol ;
	int control = 0 ;
    int controlColumn = 0 ;
    strcontrol.assign(buffer) ;
    while ( controlColumn < 2 ) {

        string controltrans ;
        while ( control < strcontrol.size() && (strcontrol.at(control) == ' ' || strcontrol.at(control) == '\t') )
            control++ ;
        while ( strcontrol.size() > control && ( strcontrol.at(control) != ' ' && strcontrol.at(control) != '\t' ) ) {
            controltrans = controltrans + strcontrol.at(control) ;
            control ++ ;
        } // while

        if ( controlColumn == 0 )
            gMethod = atoi(controltrans.c_str()) ;
        else if ( controlColumn == 1 )
            gTimeSlice = atoi(controltrans.c_str()) ;
        controlColumn++ ;

    } // while

	inFile.getline( buffer, 255, '\n' ) ; // skip the name line ;

	// Get All ID ...... ******************************************************************************************** //
    while ( !inFile.eof() ) {

		while ( inFile.getline( buffer, 255, '\n' ) ) {

            // cout << "?"  ;
			Process tempInput ;
            string str ;

			str.assign( buffer ) ;

            int index = 0 ;
            int column = 0 ;
            while ( column < 4 ) {

                while ( index < str.size() && (str.at(index) == ' ' || str.at(index) == '\t') )
                    index++ ;

                string transStr ;
                while ( str.size() > index && ( str.at(index) != ' ' && str.at(index) != '\t' ) ) {

                    transStr = transStr + str.at(index) ;
                    index ++ ;

                } // while

                // cout << transStr << endl ;
                if ( column == 0 )
                    tempInput.ID = atoi(transStr.c_str()) ;
                else if ( column == 1 )
                    tempInput.CPUBurst = atoi(transStr.c_str()) ;
                else if ( column == 2 )
                    tempInput.ArrivalTime = atoi(transStr.c_str()) ;
                else if ( column == 3 )
                    tempInput.Prior = atoi(transStr.c_str()) ;

                column++ ;

            } // while

			list.push_back(tempInput) ;

		} // while

	} // while

	inFile.close() ;

} // getInput()


void IDsort( vector<Process> &list ) {

    int i, j;
    for( i = 0 ; i < list.size() ; i++ ) {

        for( j = 0 ; j < list.size()-1-i ; j++ ){
            if ( list.at(j).ID > list.at(j+1).ID )
                swap( list.at(j), list.at(j+1) ) ;
        } // for

    } // for

} // IDsort

void PrintResult( string filename, vector<Process> list ) {

    cout << "Waiting Time" << endl ;
    cout << "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority" << endl ;
    cout << "===========================================================" << endl ;

    for( int i = 0 ; i < list.size() ; i++ ) {

        cout << list.at(i).ID << "\t" << list.at(i).fcfs[0] << "\t" << list.at(i).rr[0] << "\t" << list.at(i).psjf[0] << "\t" << list.at(i).nsjf[0] << "\t" << list.at(i).pp[0] << endl ;

    } // for

    cout << "===========================================================" << endl ;
    cout << "Turnaround Time" << endl ;
    cout << "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority" << endl ;
    cout << "===========================================================" << endl ;

    for( int i = 0 ; i < list.size() ; i++ ) {

        cout << list.at(i).ID << "\t" << list.at(i).fcfs[1] << "\t" << list.at(i).rr[1] << "\t" << list.at(i).psjf[1] << "\t" << list.at(i).nsjf[1] << "\t" << list.at(i).pp[1] << endl ;

    } // for

    cout << "===========================================================" << endl ;

    ofstream outfile( filename.c_str(), ios::app ) ;

    outfile << "Waiting Time" << endl ;
    outfile << "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority" << endl ;
    outfile << "===========================================================" << endl ;

    for( int i = 0 ; i < list.size() ; i++ ) {

        outfile << list.at(i).ID << "\t" << list.at(i).fcfs[0] << "\t" << list.at(i).rr[0] << "\t" << list.at(i).psjf[0] << "\t" << list.at(i).nsjf[0] << "\t" << list.at(i).pp[0] << endl ;

    } // for

    outfile << "===========================================================" << endl ;
    outfile << "Turnaround Time" << endl ;
    outfile << "ID\tFCFS\tRR\tPSJF\tNPSJF\tPriority" << endl ;
    outfile << "===========================================================" << endl ;

    for( int i = 0 ; i < list.size() ; i++ ) {

        outfile << list.at(i).ID << "\t" << list.at(i).fcfs[1] << "\t" << list.at(i).rr[1] << "\t" << list.at(i).psjf[1] << "\t" << list.at(i).nsjf[1] << "\t" << list.at(i).pp[1] << endl ;

    } // for

    outfile << "===========================================================" << endl ;

    outfile.close() ;

} // PrintResult()

char IDtransform( int ID ) {

    char strID ;

    if ( ID < 10 )
        strID = ID + 48 ;
    else if ( ID >= 10 )
        strID = ID + 55 ;


    return strID ;

} // IDtransform()

void FCFS( vector<Process> &list ) {


    ofstream outfile( filename.c_str(), ios::app ) ;

    cout << "==    FCFS==" << endl << "-" ;
    outfile << "==    FCFS==" << endl << "-" ;

    bool done[list.size()] ;
    bool thefirstProcess = true ;
    int allWait = 0 ;

    for ( int i = 0 ; i < list.size() ; i++ )
        done[i] = false ;

    for ( int doneCounter = 0 ; doneCounter < list.size() ; doneCounter++ ) {

        // 1.find earlier arrive ;
        int nowDo = arrivalSmallest( list, done ) ;

        if ( thefirstProcess ) {
            allWait = list.at(nowDo).ArrivalTime ;
            thefirstProcess = false ;
        } // if

        // 2.
        int currentCpuBurst = list.at(nowDo).CPUBurst ;
        list.at(nowDo).fcfs[0] = allWait - list.at(nowDo).ArrivalTime ;
        while ( currentCpuBurst > 0 ) {

            currentCpuBurst = currentCpuBurst - gTimeSlice ;
            for ( int i = 0 ; i < gTimeSlice && currentCpuBurst >= 0 ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            } // for
            allWait = allWait + gTimeSlice ;

        } // while
        if ( currentCpuBurst < 0 ) {
            for ( int i = -currentCpuBurst ; i < gTimeSlice ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            } // for
            allWait = allWait + currentCpuBurst ;
        } // if
        list.at(nowDo).fcfs[1] = allWait - list.at(nowDo).ArrivalTime ;
        done[nowDo] = true ;

    } // for

    cout << endl ;
    outfile << endl ;
    outfile.close();

} // FCFS

int arrivalSmallest( vector<Process> list, bool done[] ) {

    int smallindex = 0 ;
    int smallestArrivalTime = 9999 ;

    for ( int i = 0 ; i < list.size() ; i++ ) {

        if ( list.at(i).ArrivalTime < smallestArrivalTime && !done[i] ) {

            smallindex = i ;
            smallestArrivalTime = list.at(i).ArrivalTime ;

        } // if

    } // for

    return smallindex ;

} // arrivalSmallest()

void NSJF( vector<Process> &list ) {

    ofstream outfile( filename.c_str(), ios::app ) ;

    cout << "==Non-PSJF==" << endl << "-" ;
    outfile << "==Non-PSJF==" << endl << "-" ;
    bool done[list.size()] ;

    int walkingTime = 0 ;
    int nowDo = 0 ;
    int arriveCheck = 0 ;
    bool findone = false ;

    for ( int i = 0 ; i < list.size() ; i++ )
        done[i] = false ;

    for ( int doneCounter = 0 ; doneCounter < list.size() ; doneCounter++ ) {

        while ( true ) {

            int tempBurstTime = 9999 ;
            int tempArriveTime = 9999 ;
            for ( arriveCheck = 0 ; arriveCheck < list.size() ; arriveCheck++ ) { // find the arrive & the smallest burst time ;


                if ( !done[arriveCheck] && list.at(arriveCheck).ArrivalTime <= walkingTime ) {

                    if ( tempBurstTime == list.at(arriveCheck).CPUBurst && tempArriveTime > list.at(arriveCheck).ArrivalTime ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).CPUBurst ;
                        tempArriveTime = list.at(arriveCheck).ArrivalTime ;
                        findone = true ;

                    } // if
                    else if ( tempBurstTime > list.at(arriveCheck).CPUBurst ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).CPUBurst ;
                        tempArriveTime = list.at(arriveCheck).ArrivalTime ;
                        findone = true ;

                    } // if


                } // if

            } // for

            // cout << "wt" << walkingTime << endl ;

            if ( findone )
                break ;

            walkingTime++ ;

        } // while

        list.at(nowDo).nsjf[0] = walkingTime - list.at(nowDo).ArrivalTime ;
        int currentCpuBurst = list.at(nowDo).CPUBurst ;
        while ( currentCpuBurst > 0 ) {

            currentCpuBurst = currentCpuBurst - gTimeSlice ;
            for ( int i = 0 ; i < gTimeSlice && currentCpuBurst >= 0 ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            } // for
            walkingTime = walkingTime + gTimeSlice ;

        } // while
        if ( currentCpuBurst < 0 ) {
            for ( int i = -currentCpuBurst ; i < gTimeSlice ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            } // for
            walkingTime = walkingTime + currentCpuBurst ;
        } // if
        list.at(nowDo).nsjf[1] = walkingTime - list.at(nowDo).ArrivalTime ;

        findone = false ;
        done[nowDo] = true ;


    } // for

    cout << endl ;
    outfile << endl ;
    outfile.close();

} // NSJF



void PSJF( vector<Process> &list ) {

    ofstream outfile( filename.c_str(), ios::app ) ;

    cout << "==    PSJF==" << endl << "-" ;
    outfile << "==    PSJF==" << endl << "-" ;
    bool done[list.size()] ;


    int walkingTime = 0 ;
    int nowDo = 0 ;
    int arriveCheck = 0 ;
    bool findone = false ;
    bool stillworking = false ;

    for ( int i = 0 ; i < list.size() ; i++ )
        done[i] = false ;
    for ( int i = 0 ; i < list.size() ; i++ )
        list.at(i).leave = list.at(i).CPUBurst ;

    for ( int doneCounter = 0 ; doneCounter < list.size() ;  ) {

        while ( true ) {

            int tempBurstTime = 9999 ;
            int tempArriveTime = 9999 ;
            for ( arriveCheck = 0 ; arriveCheck < list.size() ; arriveCheck++ ) { // find the arrive & the smallest burst time ;


                if ( !done[arriveCheck] && list.at(arriveCheck).ArrivalTime <= walkingTime ) {

                    if ( tempBurstTime == list.at(arriveCheck).leave && tempArriveTime > list.at(arriveCheck).ArrivalTime && !stillworking ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).leave ;
                        tempArriveTime = list.at(arriveCheck).ArrivalTime ;
                        findone = true ;

                    } // if
                    else if ( tempBurstTime > list.at(arriveCheck).leave ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).leave ;
                        tempArriveTime = list.at(arriveCheck).ArrivalTime ;
                        findone = true ;

                    } // if


                } // if

            } // for

            // cout << "wt" << walkingTime << endl ;

            if ( findone )
                break ;

            walkingTime++ ;

        } // while

        // list.at(nowDo).psjf[0] = walkingTime - list.at(nowDo).ArrivalTime ;

        if ( list.at(nowDo).leave > 0 ) {

            list.at(nowDo).leave = list.at(nowDo).leave - 1 ;
            // for ( int i = 0 ; i < gTimeSlice && list.at(nowDo).leave >= 0 ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            // } // for
            walkingTime = walkingTime + 1 ;

        } // while

        if ( list.at(nowDo).leave < 0 ) {
            walkingTime = walkingTime + list.at(nowDo).leave ;
            for ( int i = -list.at(nowDo).leave ; i < gTimeSlice ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            } // for
        } // if
        list.at(nowDo).psjf[0] = walkingTime - list.at(nowDo).ArrivalTime - list.at(nowDo).CPUBurst ;
        list.at(nowDo).psjf[1] = walkingTime - list.at(nowDo).ArrivalTime ;

        findone = false ;

        if ( list.at(nowDo).leave <= 0 ) {
            doneCounter++ ;
            done[nowDo] = true ;
            stillworking = false ;
        } // if
        else
            stillworking = true ;

    } // for

    cout << endl ;
    outfile << endl ;
    outfile.close();

} // PSJF


void RR( vector<Process> &list ) {

    ofstream outfile( filename.c_str(), ios::app ) ;

    cout << "==      RR==" << endl << "-" ;
    outfile << "==      RR==" << endl << "-" ;

    bool done[list.size()] ;

    int walkingTime = 0 ;
    int nowDo = 0 ;
    int arriveCheck = 0 ;
    bool findone = false ;

    for ( int i = 0 ; i < list.size() ; i++ )
        done[i] = false ;
    for ( int i = 0 ; i < list.size() ; i++ )
        list.at(i).leave = list.at(i).CPUBurst ;
    for ( int i = 0 ; i < list.size() ; i++ )
        list.at(i).rr[2] = list.at(i).ArrivalTime ;


    for ( int doneCounter = 0 ; doneCounter < list.size() ;  ) {

        while ( true ) {

            int tempBurstTime = 9999 ;
            int tempArriveTime = 9999 ;
            int tempRequene = 9999 ;
            for ( arriveCheck = 0 ; arriveCheck < list.size() ; arriveCheck++ ) { // find the arrive & the smallest burst time ;


                if ( !done[arriveCheck] && list.at(arriveCheck).rr[2] <= walkingTime ) {

                    if ( tempArriveTime == list.at(arriveCheck).rr[2] && tempRequene > list.at(arriveCheck).rr[3] ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).leave ;
                        tempArriveTime = list.at(arriveCheck).rr[2] ;
                        tempRequene = list.at(arriveCheck).rr[3] ;
                        findone = true ;

                    } // if

                    if ( tempArriveTime > list.at(arriveCheck).rr[2] ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).leave ;
                        tempArriveTime = list.at(arriveCheck).rr[2] ;
                        tempRequene = list.at(arriveCheck).rr[3] ;
                        findone = true ;

                    } // if

                } // if

            } // for


            if ( findone )
                break ;

            walkingTime++ ;

        } // while


        if ( list.at(nowDo).leave > 0 ) {

            list.at(nowDo).leave = list.at(nowDo).leave - gTimeSlice ;
            for ( int i = 0 ; i < gTimeSlice && list.at(nowDo).leave >= 0 ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            } // for
            walkingTime = walkingTime + gTimeSlice ;

        } // if

        if ( list.at(nowDo).leave < 0 ) {

            walkingTime = walkingTime + list.at(nowDo).leave ;
            for ( int i = -list.at(nowDo).leave ; i < gTimeSlice ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            } // for

        } // if
        else {

            list.at(nowDo).rr[2] = walkingTime ;
            list.at(nowDo).rr[3] = list.at(nowDo).rr[3] + 1 ;

        } // else
        list.at(nowDo).rr[0] = walkingTime - list.at(nowDo).ArrivalTime - list.at(nowDo).CPUBurst ;
        list.at(nowDo).rr[1] = walkingTime - list.at(nowDo).ArrivalTime ;

        findone = false ;

        if ( list.at(nowDo).leave <= 0 ) {
            doneCounter++ ;
            done[nowDo] = true ;
        } // if

    } // for

    cout << endl ;
    outfile << endl ;
    outfile.close();

} // RR

void PP( vector<Process> &list ) {

    ofstream outfile( filename.c_str(), ios::app ) ;

    cout << "== Priority==" << endl << "-" ;
    outfile << "== Priority==" << endl << "-" ;
    bool done[list.size()] ;


    int walkingTime = 0 ;
    int nowDo = 0 ;
    int arriveCheck = 0 ;

    bool findone = false ;
    bool stillworking = false ;
    int tempBurstTime = 9999 ;
    int tempArriveTime = 9999 ;
    int tempPrior = 9999 ;
    int tempRequene = 9999 ;

    for ( int i = 0 ; i < list.size() ; i++ )
        done[i] = false ;
    for ( int i = 0 ; i < list.size() ; i++ )
        list.at(i).leave = list.at(i).CPUBurst ;

    for ( int doneCounter = 0 ; doneCounter < list.size() ;  ) {

        while ( true ) {

            tempBurstTime = 9999 ;
            tempArriveTime = 9999 ;
            tempRequene = 9999 ;

            for ( arriveCheck = 0 ; arriveCheck < list.size() ; arriveCheck++ ) { // find the arrive & the smallest burst time ;


                if ( list.at(arriveCheck).ArrivalTime <= walkingTime && !done[arriveCheck] ) {

                    if ( tempPrior > list.at(arriveCheck).Prior ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).leave ;
                        tempArriveTime = list.at(arriveCheck).ArrivalTime ;
                        tempPrior = list.at(arriveCheck).Prior ;
                        tempRequene = list.at(arriveCheck).pp[2] ;
                        findone = true ;

                    } // if

                    else if ( tempPrior == list.at(arriveCheck).Prior && list.at(arriveCheck).pp[2] == 0 && tempArriveTime > list.at(arriveCheck).ArrivalTime && !stillworking ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).leave ;
                        tempArriveTime = list.at(arriveCheck).ArrivalTime ;
                        tempPrior = list.at(arriveCheck).Prior ;
                        tempRequene = list.at(arriveCheck).pp[2] ;
                        findone = true ;

                    } // if

                    else if ( tempPrior == list.at(arriveCheck).Prior && list.at(arriveCheck).pp[2] == 0 && tempRequene != 0 && !stillworking ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).leave ;
                        tempArriveTime = list.at(arriveCheck).ArrivalTime ;
                        tempPrior = list.at(arriveCheck).Prior ;
                        tempRequene = list.at(arriveCheck).pp[2] ;
                        findone = true ;

                    } // else if
                    else if ( tempPrior == list.at(arriveCheck).Prior && tempArriveTime > list.at(arriveCheck).ArrivalTime && tempRequene != 0 && !stillworking ) {

                        nowDo = arriveCheck ;
                        tempBurstTime = list.at(arriveCheck).leave ;
                        tempArriveTime = list.at(arriveCheck).ArrivalTime ;
                        tempPrior = list.at(arriveCheck).Prior ;
                        tempRequene = list.at(arriveCheck).pp[2] ;
                        findone = true ;

                    } // else if

                    // cout << "!now:" << nowDo << " w:" << walkingTime << "." ;

                } // if

            } // for

            // cout << "wt" << walkingTime << " nowDO:<" << list.at(nowDo).ID << ")" << IDtransform( list.at(nowDo).ID ) << " Leave:" << list.at(nowDo).leave << " " << list.at(nowDo).CPUBurst << endl ;
            // system("pause") ;

            if ( findone )
                break ;

            walkingTime++ ;

        } // while

        if ( list.at(nowDo).leave > 0 ) {

            list.at(nowDo).leave = list.at(nowDo).leave - 1 ;
            //for ( int i = 0 ; i < gTimeSlice && list.at(nowDo).leave >= 0 ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            //} // for
            walkingTime = walkingTime + 1 ;

        } // while

        if ( list.at(nowDo).leave <= 0 ) {
            walkingTime = walkingTime + list.at(nowDo).leave ;
            for ( int i = -list.at(nowDo).leave ; list.at(nowDo).leave < 0 && i < gTimeSlice ; i++ ) {
                cout << IDtransform( list.at(nowDo).ID ) ;
                outfile << IDtransform( list.at(nowDo).ID ) ;
            } // for

            doneCounter++ ;
            done[nowDo] = true ;
            tempPrior = 9999 ;
            findone = false ;
            stillworking = false ;

        } // if
        else
            stillworking = true ;

        list.at(nowDo).pp[0] = walkingTime - list.at(nowDo).ArrivalTime - list.at(nowDo).CPUBurst ;
        list.at(nowDo).pp[1] = walkingTime - list.at(nowDo).ArrivalTime ;
        list.at(nowDo).pp[2] = list.at(nowDo).pp[2] + 1 ;



    } // for

    cout << endl ;
    outfile << endl ;
    outfile.close();

} // PP

