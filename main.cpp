#include <QCoreApplication>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/types.hpp>
#include<QList>
#include<QMap>
#include<QDir>
using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using namespace std;
//---Counting-the-Matches-of-Images--//
int MatchCount(Mat ClassHead, Mat Others)
{
    std::vector<DMatch> matches;
        Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
        matcher->match(ClassHead, Others, matches, Mat());
        // Sort matches by score
        std::sort(matches.begin(), matches.end());
        // Remove not so good matches
        const int numGoodMatches = matches.size() * 0.15f;//GOOD_MATCH_PERCENT
        matches.erase(matches.begin()+numGoodMatches, matches.end());
    return matches.size();
}

pair<int, int> findEntryWithLargestValue(
    map<int, int> sampleMap)
{
    pair<int, int> entryWithMaxValue
            = make_pair(0, 0);

        // Iterate in the map to find the required entry
        map<int, int>::iterator currentEntry;
        for (currentEntry = sampleMap.begin();
             currentEntry != sampleMap.end();
             ++currentEntry) {

            // If this entry's value is more
            // than the max value
            // Set this entry as the max
            if (currentEntry->second
                > entryWithMaxValue.second) {

                entryWithMaxValue
                    = make_pair(
                        currentEntry->first,
                        currentEntry->second);
            }
        }

           return entryWithMaxValue;
}

//----------Custering-Process------------------//
void Clustering(vector<Mat> Descriptors,int Iterations)
{
    map<int,int> Class1{};
    map<int,int> Class2{};
    map<int,int> Class3{};
    Descriptors.size();
    int C1=std::rand()%(Descriptors.size() +1);
    int C2=std::rand()%(Descriptors.size() +1);
    int C3=std::rand()%(Descriptors.size() +1);
    for(int i=0;i<Iterations;i++)
    {
        for(size_t i=0;i<Descriptors.size();i++)
        {
            if(MatchCount(Descriptors[C1],Descriptors[i])>10)
            {
                Class1.insert({i,MatchCount(Descriptors[C1],Descriptors[i])});
            }
            else if(MatchCount(Descriptors[C2],Descriptors[i])>10)
            {
                Class2.insert({i,MatchCount(Descriptors[C2],Descriptors[i])});
            }
            else
            {
                Class3.insert({i,MatchCount(Descriptors[C3],Descriptors[i])});
            }

        }
        pair<int, int> Cl1
                = findEntryWithLargestValue(Class1);
        C1=Cl1.first;
        pair<int, int> Cl2
                = findEntryWithLargestValue(Class2);
        C2=Cl2.first;
        pair<int, int> Cl3=findEntryWithLargestValue(Class3);
        C3=Cl3.first;
    }
    for(auto c1:Class1)
    {
        cout<<"Image Id -"<<c1.first<<" Belongs to Class1"<<endl;
    }
    for(auto c2:Class2)
    {
        cout<<"Image Id -"<<c2.first<<" Belongs to Class2"<<endl;
    }
    for(auto c3:Class3)
    {
        cout<<"Image Id -"<<c3.first<<" Belongs to Class3"<<endl;
    }
}

//--------Feature-Extraction-and-Sending-to-clustering-----------//
void StartClustering()
{
    vector<Mat> Descriptors_list;
    String Path="";
    cout<<"Please Enter the Directory Path- "<<endl;
    cin>>Path;
    cout<<"Processing---"<<endl;
    QDir directory( QString::fromStdString(Path));
    int Counter=0;
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG",QDir::Files);
    foreach(QString filename, images)
    {
        cout<<"Image Id-"<<Counter<<endl;
        cout<<Path+"/"+filename.toStdString()<<endl;
        Mat rimg=imread(Path+"/"+filename.toStdString());
        Mat img;
        cv::resize(rimg,img,Size(400,400));
        // Variables to store keypoints and descriptors
        std::vector<KeyPoint> keypoints;
        Mat descriptors;
        // Detect ORB features and compute descriptors.
        Ptr<Feature2D> orb = ORB::create(100);//MAX_FEATURES
        orb->detectAndCompute(img, Mat(), keypoints, descriptors);
        Descriptors_list.push_back(descriptors);
        Counter++;
    }
Clustering(Descriptors_list,3);
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    StartClustering();
    return a.exec();
}
