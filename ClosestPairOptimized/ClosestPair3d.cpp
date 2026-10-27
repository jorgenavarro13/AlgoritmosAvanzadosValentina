#include <bits/stdc++.h>
using namespace std;

struct Point3D{
    double x, y,z;

    Point3D(double x, double y, double z): x(x) , y(y), z(z) {}
    Point3D(){}
};

namespace {
    string inputs_folder_3D = "./tests3d";
}

double d3D (Point3D a, Point3D b){
    return pow( 
            pow(a.x-b.x, 2)+
            pow(a.y-b.y, 2)+
            pow(a.z-b.z, 2)
        , 0.5);
}

vector<Point3D> read3DFile(string filename){
    ifstream file;
    file.open(filename);
    int number_points;
    file >> number_points;
    vector<Point3D> points;
    for(int i=0; i<number_points; i++){
        double x, y, z;
        file >> x >> y >> z;
        Point3D point(x,y,z);
        points.push_back(point);
    }
    return points;
}

pair<double, pair<Point3D,Point3D> > closestPair3DBruteForce(vector<Point3D> points){
    int n = points.size();
    double mindist = DBL_MAX;
    Point3D a, b;
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            
            double d = d3D(points[i], points[j]);
            if(d < mindist){
                mindist = d;
                a = points[i]; b = points[j];
            }
        }
    }
    return {mindist, {a,b}};
}  

pair<double, pair<Point3D,Point3D> > compare3DStripZ(vector<Point3D> stripZ, double minDist){
    int n = stripZ.size();
    cout<<"s:"<<n<<endl;
    double min = DBL_MAX;
    Point3D a,b;
    for(int i=0; i<n; i++){
        for(int j= i+1; j<n && abs(stripZ[i].z - stripZ[j].z) < minDist; j++){
            double d = d3D(stripZ[i],stripZ[j]);
            if(d < min){
                min=d;
                a = stripZ[i];
                b = stripZ[j];
            }
        }
    }
    return {min, {a,b}};

}

pair<double, pair<Point3D,Point3D> > compare3DStrip(vector<Point3D> stripY, vector<Point3D> stripZ){
    int m = stripY.size();
    if(m <= 3){
        return closestPair3DBruteForce(stripY);
    }

    int mid = m/2;
    Point3D midPoint = stripY[mid];

    vector<Point3D> syl (stripY.begin(), stripY.begin()+mid);
    vector<Point3D> syh (stripY.begin()+ mid, stripY.end());
    vector<Point3D> szh, szl;

    for(auto& p : stripZ){
        if(p.y <= midPoint.y){
            szl.push_back(p);
        }else{
            szh.push_back(p);
        }
    }

    pair<double, pair<Point3D,Point3D> > low = compare3DStrip(syl, szl );
    pair<double, pair<Point3D,Point3D> > high = compare3DStrip(syh, szh );
    
    pair<double, pair<Point3D,Point3D> > min = (low.first < high.first) ? low : high;

    vector<Point3D> strip;
    for(auto& p: stripZ){
        if(abs(p.y - midPoint.y) < min.first ){
            strip.push_back(p);
        }
    }

    pair<double, pair<Point3D,Point3D> > s = compare3DStripZ(strip, min.first);
    return (min.first < s.first) ? min : s;

}  

pair<double, pair<Point3D,Point3D> > closestPair3DOptimized(vector<Point3D> & Px, vector<Point3D> & Py,  vector<Point3D> & Pz){
   int n = Px.size();
   if (n <=3){
        return closestPair3DBruteForce(Px);
   }
   
   int mid = n/2;
   Point3D midPoint = Px[mid];

   vector<Point3D> Pxl (Px.begin(), Px.begin()+mid);
   vector<Point3D> Pxr (Px.begin()+mid, Px.end());
   
   vector<Point3D> Pyl, Pyr , Pzl, Pzr;
   for(Point3D p: Py){
        if(p.x <= midPoint.x){
            Pyl.push_back(p);
        }else{
            Pyr.push_back(p);
        }
   }

   for(Point3D p: Pz){
        if(p.x <= midPoint.x){
            Pzl.push_back(p);
        }else{
            Pzr.push_back(p);
        }
   }

   pair<double, pair<Point3D,Point3D> > left  = closestPair3DOptimized(Pxl,Pyl,Pzl);
   pair<double, pair<Point3D,Point3D> > right  = closestPair3DOptimized(Pxr,Pyr,Pzr);

   pair<double, pair<Point3D,Point3D> > min = (left.first < right.first) ? left :  right;

   vector<Point3D> stripY, stripZ; 

   for(auto &p: Py){
        if(abs(p.x - midPoint.x) < min.first){
            stripY.push_back(p);
        }
   }

   for(auto &p: Pz){
        if(abs(p.x - midPoint.x) < min.first){
            stripZ.push_back(p);
        }
   }

   pair<double, pair<Point3D,Point3D> > stripResult = compare3DStrip(stripY, stripZ);


   return ( min.first <= stripResult.first ) ? min : stripResult;

}   

pair<double, pair<Point3D,Point3D> > closestPair3D(vector<Point3D> points){
    vector<Point3D> Px = points,Py=points,Pz=points;

    sort(Px.begin(), Px.end(), [](Point3D a, Point3D b) {
        return a.x < b.x;
    });

    sort(Py.begin(), Py.end(), [](Point3D a, Point3D b) {
        return a.y < b.y  ;
    });

    sort(Pz.begin(), Pz.end(), [](Point3D a, Point3D b) {
        return a.z < b.z;
    });

    return closestPair3DOptimized(Px,Py ,Pz);
}  


int main(){
    for( const auto & entry : filesystem::directory_iterator(inputs_folder_3D)){
        string file = entry.path();
        cout<< file <<endl;
        /*
        
        pair<double, pair<Point3D, Point3D> > bruteForceResult = closestPair3DBruteForce(read3DFile(file));
        Point3D abf=bruteForceResult.second.first, bbf= bruteForceResult.second.second;
        double distance= bruteForceResult.first;
        cout << "Result with brute force: " <<distance <<" {"<<abf.x<<","<<abf.y<<","<<abf.z<<"} , "<< " {"<<bbf.x<<","<<bbf.y<<","<<bbf.z<<"} "<<endl;
        */



        pair<double, pair<Point3D, Point3D> > optimizedResult = closestPair3D(read3DFile(file));
        Point3D aopt=optimizedResult.second.first, bopt= optimizedResult.second.second;
        double distanceOpt= optimizedResult.first;
        cout << "Result with optimization: " <<distanceOpt <<" {"<<aopt.x<<","<<aopt.y<<","<<aopt.z<<"} , "<< " {"<<bopt.x<<","<<bopt.y<<","<<bopt.z<<"} "<<endl;
   
    }

}