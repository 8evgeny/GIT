#include "libs.hpp"

#define NO_DEB_OUT

#ifndef NO_DEB_OUT
#define DEB_FCN(x) std::cout<<std::endl<<#x<<std::endl; x
#else
#define DEB_FCN(x) x
#endif

void CheckInt(int i){
    int noname[10]{};
    std::cout <<"ufwLibrary::array<int> array(i)"<<std::endl;
    DEB_FCN(ufw::ufwArray<int> array(i);)

    DEB_FCN(ufw::baseContainer<int> test(i);)

    DEB_FCN(ufw::baseContainer<char> base1;)
//    DEB_FCN(ufwLibrary::baseContainer<int> base2(i);)
//    DEB_FCN(ufwLibrary::baseContainer<int> base3(&noname[0],10);)
//    DEB_FCN(ufwLibrary::baseContainer<int> base4(base2);)
//    DEB_FCN(ufwLibrary::baseContainer<int> base5(std::move(test));)



    DEB_FCN(ufw::ufwVector<int> vec1;)
    DEB_FCN(ufw::ufwVector<int> vec2(i);)
    DEB_FCN(ufw::ufwVector<int> vec3(vec2);)
    DEB_FCN(ufw::ufwVector<int> vec4(std::move(vec2));)
    DEB_FCN(ufw::ufwVector<char> vec5;)

    DEB_FCN(ufw::ufwString str1;)
    DEB_FCN(ufw::ufwString str2("Hi there!");)
    DEB_FCN(ufw::ufwString str3="Bye there!";)
    DEB_FCN(ufw::ufwString str4(str2);)
    DEB_FCN(ufw::ufwString str5(std::move(str2));)
    DEB_FCN(str1=str3;)
    DEB_FCN(str2 = "Another there!";)

//    ((ufwLibrary::vector<int> &)base4).resize(10000);
//    ((ufwLibrary::vector<int> &)base4).resize(10);
//    ufwLibrary::vector<int> &base4ref =
//        static_cast<ufwLibrary::vector<int> &>(base4);
//    base4ref.fit();

    //    for (int j = 0; j < 100; ++j) {
    //        DEB_FCN(vec1.append(j);)
    //    }
    //    DEB_FCN(vec1.fit();)
    //    DEB_FCN(vec1.resize(10);)

    //    for (int j = 0; j < 100; ++j) {
    //        DEB_FCN(vec5=str2+str4;)
    //    }
    //    DEB_FCN(str2.fit();)
    //    for (int j = 0; j < 100; ++j) {
    //        DEB_FCN(str2.append("VALUE XX ");)
    //    }

    //    for (int j = 0; j < 100; ++j) {
    //        DEB_FCN(str1=str2+str4;)
    //    }

    ufw::baseContainer<int>* baseptr = new ufw::baseContainer<int>;
    delete baseptr;
    std::cout <<"baseptr="<<baseptr<<"\n";
    baseptr = new ufw::baseContainer<int>[100];
    delete [] baseptr;
    std::cout <<"baseptr="<<baseptr<<"\n";
    DEB_FCN(vec5=str2+str4;)
    DEB_FCN(vec5=str2+str4;)
    DEB_FCN(base1=vec5;)
    DEB_FCN(str1=str2+str4+str3;)

    std::cout <<"str1="<<str1()<<std::endl<<"str2="<<str2()<<std::endl<<"str3="<<str3()<<std::endl<<"str4="<<str4()<<std::endl<<"str5="<<str5()<<std::endl;


//    std::cout <<"that's all"<<std::endl;
}
