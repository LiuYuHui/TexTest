#ifndef TEXTEST_INCLUDED
#define TEXTEST_INCLUDED
#include<vector>
#include<iostream>
#include<utility>
#include<string>

#define STRCAT_IMPL(s1, s2) s1##s2
#define STR_CAT(s1, s2) STRCAT_IMPL(s1, s2)
#define FUN(f) STR_CAT(f, __COUNTER__)

#define TEST_CASE_WITH_F(f, desc)\
    static void f();\
    int FUN(x) = reg(test_case(f,__FILE__,__LINE__,desc));\
    static void f()

#define TEST_CASE(...) \
    TEST_CASE_IMPL(__VA_ARGS__)

#define TEST_CASE_IMPL(d)\
    TEST_CASE_WITH_F(FUN(f),d)

#define EXPECT_EQ(...) \
     do {                                                                     \
        result e(__FILE__,__LINE__,#__VA_ARGS__);                   \
        e.eval(__VA_ARGS__);                    \
        if(e.m_failed)                    \
            addAssert(e);    \
    } while((void)0, 0)


using fun = void(*)();//resisted function

struct test_case;
struct test_data{
    unsigned numTestCases;
    unsigned numTestCasesFailed;
    int      numAsserts;
    int      numAssertsFailed;
    test_case *CurrentTestCase;
};
test_data g_test; //globol env

struct assert_case{
    const char* m_file;
    unsigned m_line;
    const char* m_description;
    bool m_failed;
};
struct test_case{
    test_case(fun f,const char* file,unsigned line,const char *description):
        m_fun (f), m_file (file), m_line (line),m_description(description) { }
    fun m_fun;
    const char* m_file;
    unsigned m_line;
    const char* m_description;
};


std::vector<test_case>& getRegContainer() {
    static std::vector<test_case> datas;
    return datas;
}


int reg(const test_case & t) {
    getRegContainer().push_back(t);
    return 0;
}

    
struct result : public assert_case{
     result(const char* file,unsigned line,const char *description){
        m_file = file;
        m_line =line;
        m_description = description;
    }
    template<typename L, typename R>
    void eval(const L &lhs,const R &rhs){
        g_test.numAsserts++;
        if(lhs == rhs){
            m_failed = false;
        }else{
            m_failed = true;
        }
    }
};

enum class Color{ red,green,yello,regular };
std::ostream& operator<< (std::ostream & os,const Color& c){
    switch (c){
        case Color::red: os<<"\e[1;31m";break;
        case Color::green: os<<"\e[0;32m";break;
        case Color::regular: os<<"\e[0m";break;
        case Color::yello: os<<"\e[0;33m";break; 
    }
    return os;
}
bool haveLogCurrentTestCase = false;
void addAssert(const assert_case& a){
    g_test.numAssertsFailed ++;
    if(!haveLogCurrentTestCase){
        std::cout<<g_test.CurrentTestCase->m_file<<":"<<g_test.CurrentTestCase->m_line<<":" <<
            std::endl<<"TEST_CASE: "<<g_test.CurrentTestCase->m_description<<std::endl;
        haveLogCurrentTestCase = true;
    }
    std::cout<<a.m_file <<":"<<a.m_line<<":"<<Color::red<<"Error: EXPECT_EQ("
        <<a.m_description <<") "<< Color::regular<<"is not correct!"<<"\n";

}

int main(){ 
    g_test.numTestCases = getRegContainer().size();
    std::cout<<std::endl
            <<Color::yello<<"======================================================"<<Color::regular<<std::endl;
   for(auto && tc : getRegContainer()){
       g_test.CurrentTestCase = &tc;
       haveLogCurrentTestCase = false;
       int prevAssertsFailed = g_test.numAssertsFailed;
       tc.m_fun();
       if(prevAssertsFailed < g_test.numAssertsFailed){
           g_test.numTestCasesFailed++;
       }
       if(haveLogCurrentTestCase)
       std::cout<<std::endl
            <<Color::yello<<"======================================================"<<Color::regular<<std::endl;
   }
   std::cout<<"test cases"<<"      "<<g_test.numTestCases<<"|      "<<Color::green<<g_test.numTestCases-g_test.numTestCasesFailed<<" passed"
        <<Color::regular<<"|     "<<Color::red
        <<g_test.numTestCasesFailed<<" failed"<<Color::regular<<std::endl;
   std::cout<<"assertions"<<"      "<<g_test.numAsserts<<"|      "<<Color::green<<g_test.numAsserts-g_test.numAssertsFailed<<" passed"
         <<Color::regular<<"|     "<<Color::red
        <<g_test.numAssertsFailed<<" failed"<<Color::regular<<std::endl;
}

#endif 

