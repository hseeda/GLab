#pragma once
#pragma warning(disable:4244)
#pragma warning(disable:4311)
#pragma warning(disable:4302)
// must be c++17 or later
#define NOMINMAX 1
#define _max(a,b) (((a) > (b)) ? (a) : (b))
#define _min(a,b) (((a) < (b)) ? (a) : (b))
//---------------------------------------------------------------------------------------------------- stdc
#include    <windows.h>
#include	<algorithm>						
#include	<algorithm>	    //	Necessary	for	std::clamp	
#include    <array>
#include	<atomic>	    //	std::atomic				
#include	<cctype>	    					
#include	<chrono>	    //	std::chrono				
#include	<cstdint>	    //	std::int_fast64_t,	std::uint_fast32_t	
#include	<cstdlib>	    //	std::size_t
#include	<filesystem>    						
#include    <fcntl.h>
#include	<format>        //  std::format
#include	<fstream>	    					
#include	<functional>    //	std::function	
#include	<future>	    //	std::future,	std::promise			
#include    <io.h>
#include	<iomanip>	    //	std::setw							
#include	<iostream>	    //	std::cout,	std::ostream			
#include	<limits>	    					
#include	<map>		    				
#include	<memory>	    //	std::shared_ptr,	std::unique_ptr			
#include	<mutex>	        //	std::mutex,	std::scoped_lock			
#include	<optional>	    					
#include	<queue>	        //	std::queue				
#include	<set>		    				
#include	<sstream>	    					
#include	<stdexcept>	    //	Necessary	for	std::out_of_range		
#include	<stdio.h>	    //	printf				
#include	<string>	    					
#include	<thread>	    //	std::this_thread,	std::thread			
#include	<type_traits>	//	std::common_type_t,	std::decay_t,	std::enable_if_t,	std::is_void_v,	std::invoke_result_t
#include	<typeinfo>						
#include	<unordered_map>						
#include	<utility>	    //	std::move				
#include	<vector>		
//----------------------------------------------------------------------------------------------------
#define KNRM  "\x1B[0;0m"
#define KRED  "\x1B[7;31m"
#define KLRED  "\x1B[1;31m"
#define KGRN  "\x1B[7;32m"
#define KLGRN  "\x1B[1;32m"
#define KYEL  "\x1B[7;33m"
#define KLYEL  "\x1B[1;33m"
#define KLBLU  "\x1B[1;34m"
#define KBLU  "\x1B[7;34m"
#define KMAG  "\x1B[7;35m"
#define KLMAG  "\x1B[1;35m"
#define KCYN  "\x1B[7;36m"
#define KLCYN  "\x1B[1;36m"
#define KWHT  "\x1B[0;37m"
#define KLYE  "\x1B[0;93m"

#define BNRM  "\033[0m"
#define BRED  "\033[31m"
#define BGRN  "\033[3;42;30m"
#define BYEL  "\033[3;43;30m"
#define BBLU  "\033[3;44;30m"
#define BMAG  "\033[3;45;30m"
#define BCYN  "\033[3;104;30m"
#define BWHT  "\033[1;47;35m"

#define _tc_reset	KNRM
#define _tc_lr		KLRED
#define _tc_r		KRED
#define _tc_ly		KLYEL
#define _tc_y		KYEL
#define _tc_lg      KLGRN
#define _tc_g       KGRN
#define _tc_lb      KLBLU
#define _tc_b       KBLU
#define _tc_lc      KLCYN
#define _tc_c       KCYN

#define _W_ 12
#define double1 Vector1D<double>
#define double2 Vector2D<double>
#define double3 Vector1D<double>
//#define int1	Vector1D<int>
//#define int2	Vector2D<int>

inline static const WORD MAX_CONSOLE_LINES = 500;
//====================================================================================================
//#define strmap std::map <std::string, std::string>
//#define p1(a1)           std::cout << std::setw(20) << a1 << std::endl
//#define p2(a1, a2)       std::cout << std::setw(20) << a1 << std::setw(20) << a2 << std::endl
//#define p3(a1, a2, a3)   std::cout << std::setw(20) << a1 << std::setw(20) << a2 << std::setw(20) << a3 << std::endl
typedef std::uint_fast32_t ui32;
typedef std::uint_fast64_t ui64;
typedef std::uint_fast32_t ui32;
typedef std::uint_fast64_t ui64;
namespace fs = std::filesystem;
//====================================================================================================
inline const std::string WHITESPACE = " \n\r\t\f\v";
inline int      clampI(int v, int st, int en);
inline float    clampF(float v, float st, float en);
inline int      clampIcycle(int v, int st, int en);
//====================================================================================================
template <typename... T> void print(const T &...items) { (std::cout << ... << items); };
//------
template <typename... T> void println(const T &...items) { print(items..., '\n'); };
//------
template <typename... T> void printr(const T &...items) { print(items..., '\r'); };
//------
template <typename... T> void	_p(const T &...items) { (std::cout << ... << items); };
//------
template <typename... T> void	_pl(const T &...items) { _p(items..., '\n'); };
//------
template <typename... T> void	_pe(const T &...items) { (std::cerr << ... << items); };
//------
template <typename... T> void	_ple(const T &...items) { _pe(items..., '\n'); };

template <typename T> void _pv(const std::vector<T>& v) {int c = 0; for (auto& i : v) { std::cout << std::setw(5) << c << ": " << i << "\n"; c++;}};
//------
template <typename FIRST, typename... REST> 
inline void        _hlog(const FIRST& a, const REST &...items)
{	std::cout << a;	_hlog(items...);};
//------
template <typename T1> void _flog(T1& a1)
{	std::cout << "  " << std::setw(_W_) << a1 << "\n";};
//------
template <typename T1, typename T2 > void _flog(T1& a1, T2& a2)
{	std::cout << "  " << std::setw(_W_) << a1 << std::setw(_W_) << a2 << "\n";};
//------
template <typename T1, typename T2, typename T3> void _flog(T1& a1, T2& a2, T3& a3)
{	std::cout << "  " << std::setw(_W_) << std::right << a1 << std::setw(3) << a2 << std::setw(_W_) << std::left << a3 << "\n";};
//====================================================================================================
std::vector<std::string> split(const char* str, char separator);
//====================================================================================================
inline void findReplaceAll(std::string &data, std::string search, std::string replaceStr)
{
	while(data.find(search) != std::string::npos) {
		data.replace(data.find(search), search.size(), replaceStr);
	}
}
//====================================================================================================
template <typename T> class a1d
{
public:
    int m;	T* c;
    a1d(int _m) { m = _m; c = new T(m); }
    a1d() {}
    ~a1d() { delete c; }
    T operator()(int i) { return c[i]; }
    void zero() { for (int i = 0; i < m; i++) { *c + i = 0; } }
    void set_value(T v) { for (int i = 0; i < m; i++) { *c + i = v; } }
};
//====================================================================================================
template <typename T> class a2d
{
public:
    int m, n; T* c;
    a2d() {}
    a2d(int _m, int _n) { m = _m; n = _n; c = new T(n * m); }
    ~a2d() { delete c; }
    T operator()(int i, int j) { return c[i + n * j]; }
    void zero() { for (int i = 0; i < m * n; i++) { *c + i = 0; } }
    void set_value(T v) { for (int i = 0; i < m * n; i++) { *c + i = v; } }
};
//====================================================================================================
template <typename T> class a3d
{
public:
    int m, n, o; T* c;
    a3d() {}
    a3d(int _m, int _n, int _o) { m = _m; n = _n; o = _o; c = new T(n * m * o); }
    ~a3d() { delete c; }
    T operator()(int i, int j, int k) { return c[i + n * j + m * n * k]; }
    void zero() { for (int i = 0; i < m * n * o; i++) { *c + i = 0; } }
    void set_value(T v) { for (int i = 0; i < m * n * o; i++) { *c + i = v; } }
};
//====================================================================================================
class HTimer
{
    typedef std::int_fast64_t i64;
public:
    HTimer(int i = 0):timer_number(i) { start(); }
    inline void start() { 
        start_time = std::chrono::steady_clock::now();
        //std::cout << std::format(">>> Timer {:04i} >>>\n", timer_number);
    }
	inline void stop() {
        elapsed_time = std::chrono::steady_clock::now() - start_time; 
		//std::cout << std::format("<<< Timer {:04i}  <<< Elapsed time: {:>10.3f} us\n", timer_number,elapsed_time);
	}

	inline i64  elapsed_s() { 
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count(); 
    }
	inline i64  elapsed_ms() { 
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time).count(); 
    }
    inline i64 ms() const { 
        return (std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time)).count(); 
    }
    int fps() { 
        stop(); 
        int i = 1000000 / ms(); 
        start(); 
        return i;
    }
private:
    int timer_number;
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = std::chrono::duration<double>::zero();
};
//====================================================================================================
class FileParser
{
public:
    std::vector <std::string> data;
    std::string filename;
    std::string param;
    std::string value;

    FileParser(std::string filename) : filename(filename) {}

    FileParser() {}

    ~FileParser() { closeFile(); }

    void parse() {
        openFile();

        while (getNextParam()) {
            std::cout << param << " : " << value << std::endl;
            m_map[param] = value;
        }
        closeFile();
    }

    bool getParam(std::string p, std::string& v) {
        openFile();
        toupper(p);
        while (getNextParam()) {
            if (p == param) {
                v = value;
                closeFile();
                return true;
            }
        }
        closeFile();
        return false;
    }

    bool getInt(std::string p, int& v) {
        bool flag = getParam(p);
        if (flag) {
            v = atoi(value.c_str());
            return true;
        }
        return false;
    }

    bool getDouble(std::string p, double& v) {
        bool flag = getParam(p);
        if (flag) {
            v = atof(value.c_str());
            return true;
        }
        return false;
    }

    bool getBool(std::string p, bool& v) {
        bool flag = getParam(p);
        if (flag) {
            if (value == "T") v = true;
            if (value == "F") v = false;
            return true;
        }
        return false;
    }

private:
    std::string m_line;
    std::string m_token;
    std::ifstream m_file;
    std::map<std::string, std::string> m_map;

    void parseParam() {
        std::istringstream iss(m_line);
        std::getline(iss, m_token, '=');		param = m_token;
        std::getline(iss, m_token, '=');		value = m_token;
        trim(param);
        for (auto& c : param) c = std::toupper(c);
        trim(value);
    }

    bool getParam(std::string& p) {
        openFile();
        toupper(p);
        while (getNextParam()) {
            if (p == param) {
                closeFile();
                return true;
            }
        }
        closeFile();
        return false;
    }

    bool getNextParam() {
        int c = 0;
        int flag = 1;
        if (!m_file.is_open()) m_file.open(filename);
        while (std::getline(m_file, m_line) && flag) {
            ltrim(m_line);
#ifdef _DEBUG
            //std::cout << ">| " << m_line << std::endl;
#endif
            // skip commented lines
            if (m_line[0] != '!') {
                std::istringstream iss(m_line);
                c = 0;
                while (std::getline(iss, m_token, '=')) {
                    if (m_token.length() > 0) {
                        c++;
                    }
                }
                if (c == 2) {
                    parseParam();
                    flag = 0;
                    return true;
                }
            }
        }
        return false;
    }

    void openFile() {
        if (m_file.is_open()) {
            m_file.close();
        }
        m_file.open(filename);
        if (!m_file.is_open()) {
            std::cout << "ERROR: File " << filename << " not found" << std::endl;
            exit(1);
        }
    }
    void closeFile() {
        if (m_file.is_open()) {
            m_file.close();
        }
    }

    static inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string& s) {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    static inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    static inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    static inline std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }

    static inline void toupper(std::string& s) {
        for (auto& c : s) c = std::toupper(c);
    }
};
//====================================================================================================
class OFile {
public:
    std::string filename;
    OFile(std::string filename) : filename(filename) {}
    OFile() {}

    ~OFile() { closeFile(); }

    void openFile() {
        if (m_file.is_open()) {
            m_file.close();
        }
        m_file.open(filename);
        if (!m_file.is_open()) {
            std::cout << "ERROR: File " << filename << " not found" << std::endl;
            exit(1);
        }
    }

    void openFileAppend()
    {
        if (m_file.is_open()) {
            m_file.close();
        }
        m_file.open(filename, std::ios::app);
        if (!m_file.is_open()) {
            std::cout << "ERROR: File " << filename << " not found" << std::endl;
            exit(1);
        }
    }

    void closeFile() {
        if (m_file.is_open()) {
            m_file.close();
        }
    }

    void writeLine(std::string line) {
        if (!m_file.is_open()) openFile();
        m_file << line << std::endl;
    }


private:
    std::ofstream m_file;
};
//====================================================================================================
class IFile {
public:
    std::string filename;
    IFile(std::string filename) : filename(filename) {}
    IFile() {}

    ~IFile() { closeFile(); }

    bool openFile() {
        if (m_file.is_open()) {
            m_file.close();
        }
        m_file.open(filename);
        if (!m_file.is_open()) {
            std::cout << "ERROR: File " << filename << " not found" << std::endl;
            exit(1);
            return false;
        }
        return true;
    }

    void closeFile() {
        if (m_file.is_open()) {
            m_file.close();
        }
    }

    bool getNextLine(std::string& line) {
        if (!m_file.is_open()) openFile();
        if (std::getline(m_file, line)) {
            return true;
        }
        return false;
    }
    int countLine(std::string &line, char separator = ' ')
    {
        int c = 0;
        std::istringstream iss(line);
        std::string token;
        c = 0;
        while (std::getline(iss, token, separator)) {
            if (token.length() > 0) {
                c++;
            }
        }
        return c;
    }

    int countLines()
    {
        int c = 0;
        std::string s;
        closeFile();
        openFile();
        c = 0;
        while (getNextLine(s)) c++;
        return c;
    }

    // remember to clean vector before sending it
    int parseNextLineD(std::vector<double> &v, char separator = ' ')
    {
        int c = 0;
        std::string line;
        if (std::getline(m_file, line)) {
            std::istringstream iss(line);
            std::string token;
            c = 0;
            while (std::getline(iss, token, separator)) {
                if (token.length() > 0) {
                    c++;
                    v.push_back(std::stod(token));
                }
            }
        }
        return c;
    }


    int parseNextLineD(char separator = ' ', double& d1 = m_d, double& d2 = m_d, double& d3 = m_d, double& d4 = m_d, double& d5 = m_d,
        double& d6 = m_d, double& d7 = m_d, double& d8 = m_d, double& d9 = m_d, double& d20 = m_d, double& d11 = m_d,
        double& d12 = m_d, double& d13 = m_d, double& d14 = m_d, double& d15 = m_d, double& d16 = m_d)
    {
        int c = 0;
        if (!m_file.is_open()) openFile();
        std::string line;
        if (std::getline(m_file, line)) {
            std::istringstream iss(line);
            std::string token;
            c = 0;
            while (std::getline(iss, token, separator)) {
                if (token.length() > 0) {
                    c++;
                    if (c == 1) {
                        d1 = std::stod(token);
                    }
                    else if (c == 2) {
                        d2 = std::stod(token);
                    }
                    else if (c == 3) {
                        d3 = std::stod(token);
                    }
                    else if (c == 4) {
                        d4 = std::stod(token);
                    }
                    else if (c == 5) {
                        d5 = std::stod(token);
                    }
                    else if (c == 6) {
                        d6 = std::stod(token);
                    }
                    else if (c == 7) {
                        d7 = std::stod(token);
                    }
                    else if (c == 8) {
                        d8 = std::stod(token);
                    }
                    else if (c == 9) {
                        d9 = std::stod(token);
                    }
                    else if (c == 10) {
                        d20 = std::stod(token);
                    }
                    else if (c == 11) {
                        d11 = std::stod(token);
                    }
                    else if (c == 12) {
                        d12 = std::stod(token);
                    }
                    else if (c == 13) {
                        d13 = std::stod(token);
                    }
                    else if (c == 14) {
                        d14 = std::stod(token);
                    }
                    else if (c == 15) {
                        d15 = std::stod(token);
                    }
                    else if (c == 16) {
                        d16 = std::stod(token);
                    }
                }
            }
        }
        return c;
    }

    int parseNextLineI(char separator = ' ', int& d1 = m_i, int& d2 = m_i, int& d3 = m_i, int& d4 = m_i, int& d5 = m_i,
        int& d6 = m_i, int& d7 = m_i, int& d8 = m_i, int& d9 = m_i, int& d20 = m_i, int& d11 = m_i,
        int& d12 = m_i, int& d13 = m_i, int& d14 = m_i, int& d15 = m_i, int& d16 = m_i)
    {
        int c = 0;
        if (!m_file.is_open()) openFile();
        std::string line;
        if (std::getline(m_file, line)) {
            std::istringstream iss(line);
            std::string token;
            c = 0;
            while (std::getline(iss, token, separator)) {
                if (token.length() > 0) {
                    c++;
                    if (c == 1) {
                        d1 = std::stoi(token);
                    }
                    else if (c == 2) {
                        d2 = std::stoi(token);
                    }
                    else if (c == 3) {
                        d3 = std::stoi(token);
                    }
                    else if (c == 4) {
                        d4 = std::stoi(token);
                    }
                    else if (c == 5) {
                        d5 = std::stoi(token);
                    }
                    else if (c == 6) {
                        d6 = std::stoi(token);
                    }
                    else if (c == 7) {
                        d7 = std::stoi(token);
                    }
                    else if (c == 8) {
                        d8 = std::stoi(token);
                    }
                    else if (c == 9) {
                        d9 = std::stoi(token);
                    }
                    else if (c == 10) {
                        d20 = std::stoi(token);
                    }
                    else if (c == 11) {
                        d11 = std::stoi(token);
                    }
                    else if (c == 12) {
                        d12 = std::stoi(token);
                    }
                    else if (c == 13) {
                        d13 = std::stoi(token);
                    }
                    else if (c == 14) {
                        d14 = std::stoi(token);
                    }
                    else if (c == 15) {
                        d15 = std::stoi(token);
                    }
                    else if (c == 16) {
                        d16 = std::stoi(token);
                    }
                }
            }
        }

        return c;
    }

private:
    std::ifstream m_file;
    static double m_d;
    static int m_i;
};
//====================================================================================================
template <typename T> class Vector1D {
public:
    int r, count;
    T* data = 0;
    Vector1D(const int r) : r(r) {
        count = r;	data = new T[r];
#ifdef _DEBUG
        std::cout << "+|create   : " << this << std::endl;
#endif
    }

    Vector1D(const Vector1D<T>& a) : r(a.r) {
        count = r;	data = new T[r];
        memcpy(data, a.data, count * sizeof(T));
#ifdef _DEBUG
        std::cout << "+|copy     : " << this << std::endl;
#endif
    }
    Vector1D() : r(0) {
        count = 0;
        data = nullptr;
#ifdef _DEBUG
        std::cout << "+|create   : " << this << std::endl;
#endif
    }

    ~Vector1D() {
#ifdef _DEBUG
        std::cout << "-|destroy  : " << this << std::endl;
#endif
        if (data) delete[] data;
    }

    void deallocate()
    {
        if (data) delete[] data;
        data = nullptr;
        count = 0;
        r = 0;
    }

    void clear() {
        r = 0;
        count = 0;
        if (data) delete[] data;
    }

    void set(const int r) {
        this->r = r;
        count = r;
        if (data) delete[] data;
        data = new T[r];
#ifdef _DEBUG
        std::cout << "#|set      : " << this << std::endl;
#endif
    }

    void allocate(const int r) {
        this->r = r;
        count = r;
        if (data) delete[] data;
        data = new T[r];
#ifdef _DEBUG
        std::cout << "#|set      : " << this << std::endl;
#endif
    }

    void range(Vector1D<T>& a, int istart, int iend) {
        allocate(iend - istart + 1);
        for (int i = 0; i < r; i++) {
            value(i) = a(istart + i);
        }
    }


    inline Vector1D<T>& operator=(const Vector1D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op =     : " << this << "  " << &a << std::endl;
#endif
        if (check(a)) {
            memcpy(data, a.data, count * sizeof(T));
            return *this;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    void operator=(const T a) {
#ifdef _DEBUG
        std::cout << "#|op =     : " << this << "  " << &a << std::endl;
#endif
        memcpy(data, a.data, count * sizeof(T));
    }

    inline T& operator()(const int i) {
#ifdef _DEBUG
        std::cout << "#|op ()    : " << this << std::endl;
#endif
        if (i < r)	return data[i];
        std::cout << "EEROR   :" << std::endl;
        exit(999);
    }

    Vector1D<T> operator+(const Vector1D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op +     : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            Vector1D<T> b(r);
            for (int i = 0; i < count; i++) b.data[i] = a.data[i] + data[i];
            return b;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    Vector1D<T> operator*(const Vector1D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op +     : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            Vector1D<T> b(r);
            for (int i = 0; i < count; i++) b.data[i] = a.data[i] * data[i];
            return b;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }


    Vector1D<T> operator-(const Vector1D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op -     : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            Vector1D<T> b(r);
            for (int i = 0; i < count; i++) b.data[i] = data[i] - a.data[i];
            return b;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    Vector1D<T> add(const Vector1D<T>& a, const Vector1D<T>& b)
    {
#ifdef _DEBUG
        std::cout << "#|add      : " << this << " " << &a << " " << &b << std::endl;
#endif
        if (check(a) && check(b)) {
            Vector1D<T> cc(r);
            for (int i = 0; i < count; i++) cc.data[i] = a.data[i] + b.data[i];
            return cc;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(997);
    }

    void add(const Vector1D<T>& a)
    {
#ifdef _DEBUG
        std::cout << "#|add      : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            for (int i = 0; i < count; i++) data[i] += a.data[i];
        }
        std::cout << "EEROR   :" << std::endl;
        exit(996);
    }

    Vector1D<T> subtract(const Vector1D<T>& a, const Vector1D<T>& b)
    {
#ifdef _DEBUG
        std::cout << "#|subtract : " << this << " " << &a << " " << &b << std::endl;
#endif
        if (check(a) && check(b)) {
            Vector1D<T> cc(r);
            for (int i = 0; i < count; i++) cc.data[i] = a.data[i] - b.data[i];
            return cc;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(995);
    }
    void subtract(const Vector1D<T>& a)
    {
#ifdef _DEBUG
        std::cout << "#|subtract : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            for (int i = 0; i < count; i++) data[i] -= a.data[i];
        }
        std::cout << "EEROR   :" << std::endl;
        exit(994);
    }

    void zero() {
#ifdef _DEBUG
        std::cout << "0|zero     : " << this << std::endl;
#endif
        memset(data, 0, count * sizeof(T));
    }

    void p(int i, int j) {
        std::cout << " |v(" << i << "," << j << ") = " << value(i, j) << std::endl;
    }

    void p() {
        std::cout.width(12); std::cout << "|||||||||||||";
        for (int i = 0; i < r; i++) {
            std::cout.width(12); std::cout << i << "|";
        }
        std::cout << std::endl;
        std::cout.width(12); std::cout << "|||||||||||||";
        for (int i = 0; i < r; i++) {
            std::cout.width(12); std::cout << "____________" << " ";
        }
        std::cout << std::endl;
        for (int j = 0; j < r; j++) {
            std::cout.width(12); std::cout << data[j] << " ";
            std::cout << std::endl;
        }
    }

private:
    T& value(int i, int j) {
        if (i < r) return data[i];
        std::cout << "EEROR   :" << std::endl;
        exit(999);
    }

    inline bool check(const Vector1D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|check    : " << this << std::endl;
#endif
        if (r != a.r) return false;
        return true;
    }
};
//====================================================================================================
template <typename T> class Vector2D {
public:
    int r, c, count;
    T* data;
    Vector2D(const int r, const int c) : r(r), c(c) {
        count = r * c;	data = new T[r * c];
#ifdef _DEBUG
        std::cout << "+|create   : " << this << std::endl;
#endif
    }

    Vector2D(const Vector2D<T>& a) : r(a.r), c(a.c) {
        count = r * c;	data = new T[r * c];
        memcpy(data, a.data, count * sizeof(T));
#ifdef _DEBUG
        std::cout << "+|copy     : " << this << std::endl;
#endif
    }

    Vector2D() : r(0), c(0), data(nullptr) {
        count = 0;
#ifdef _DEBUG
        std::cout << "+|create   : " << this << std::endl;
#endif
    }

    ~Vector2D() {
#ifdef _DEBUG
        std::cout << "-|destroy  : " << this << std::endl;
#endif
        if (data) delete[] data;
    }

    void deallocate() {
        r = 0;
        c = 0;
        count = 0;
        if (data) delete[] data;
    }

    void clear() {
        r = 0;
        c = 0;
        count = 0;
        if (data) delete[] data;
    }

    void set(const int r, const int c) {
        this->r = r;	this->c = c;
        count = r * c;
        if (data) delete[] data;
        data = new T[r * c];
#ifdef _DEBUG
        std::cout << "#|set      : " << this << std::endl;
#endif
    }

    void allocate(const int r, const int c) {
        this->r = r;	this->c = c;
        count = r * c;
        if (data) delete[] data;
        data = new T[r * c];
#ifdef _DEBUG
        std::cout << "#|set      : " << this << std::endl;
#endif
    }
    void range(Vector2D<T>& a, int istart, int iend, int jstart, int jend) {
        //	range function:
        //		istart: start row index
        //		iend: end row index
        //		jstart: start column index
        //		jend: end column index
        //		a: output matrix
        //		a = this(istart:iend,jstart:jend)

        allocate(iend - istart + 1, jend - jstart + 1);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                value(i, j) = a(istart + i, jstart + j);
            }
        }
    }

    Vector2D<T>& operator=(const Vector2D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op =     : " << this << "  " << &a << std::endl;
#endif
        if (check(a)) {
            memcpy(data, a.data, count * sizeof(T));
            return *this;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    void operator=(const T a) {
#ifdef _DEBUG
        std::cout << "#|op =     : " << this << "  " << &a << std::endl;
#endif
        memcpy(data, a.data, count * sizeof(T));
    }

    T& operator()(const int i, const int j) {
#ifdef _DEBUG
        std::cout << "#|op ()    : " << this << std::endl;
#endif
        if (i < r && j < c)	return data[i + r * j];
        std::cout << "EEROR   :" << std::endl;
        exit(999);
    }

    Vector2D<T> operator+(const Vector2D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op +     : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            Vector2D<T> b(r, c);
            for (int i = 0; i < count; i++) b.data[i] = a.data[i] + data[i];
            return b;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    Vector2D<T> operator+(T& a) {
#ifdef _DEBUG
        std::cout << "#|op +     : " << this << " " << &a << std::endl;
#endif
        Vector2D<T> b(r, c);
        for (int i = 0; i < count; i++) b.data[i] = a + data[i];
        return b;
    }

    Vector2D<T> operator*(const Vector2D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op +     : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            Vector2D<T> b(r, c);
            for (int i = 0; i < count; i++) b.data[i] = a.data[i] * data[i];
            return b;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    Vector2D<T> operator*(T& a) {
#ifdef _DEBUG
        std::cout << "#|op +     : " << this << " " << &a << std::endl;
#endif

        Vector2D<T> b(r, c);
        for (int i = 0; i < count; i++) b.data[i] = a * data[i];
        return b;
    }


    Vector2D<T> operator-(const Vector2D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op -     : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            Vector2D<T> b(r, c);
            for (int i = 0; i < count; i++) b.data[i] = data[i] - a.data[i];
            return b;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    Vector2D<T> add(const Vector2D<T>& a, const Vector2D<T>& b)
        // function add:
        //		a: input matrix
        //		b: input matrix
        //		c: output matrix
        //		c = a + b
    {
#ifdef _DEBUG
        std::cout << "#|add      : " << this << " " << &a << " " << &b << std::endl;
#endif
        if (check(a) && check(b)) {
            Vector2D<T> cc(r, c);
            for (int i = 0; i < count; i++) cc.data[i] = a.data[i] + b.data[i];
            return cc;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(997);
    }

    void add(const Vector2D<T>& a)
        //function add:
        //	a: input matrix
        //	this = this + a
    {
#ifdef _DEBUG
        std::cout << "#|add      : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            for (int i = 0; i < count; i++) data[i] += a.data[i];
        }
        std::cout << "EEROR   :" << std::endl;
        exit(996);
    }

    Vector2D<T> subtract(const Vector2D<T>& a, const Vector2D<T>& b)
    {
#ifdef _DEBUG
        std::cout << "#|subtract : " << this << " " << &a << " " << &b << std::endl;
#endif
        if (check(a) && check(b)) {
            Vector2D<T> cc(r, c);
            for (int i = 0; i < count; i++) cc.data[i] = a.data[i] - b.data[i];
            return cc;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(995);
    }

    void subtract(const Vector2D<T>& a)
    {
#ifdef _DEBUG
        std::cout << "#|subtract : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            for (int i = 0; i < count; i++) data[i] -= a.data[i];
        }
        std::cout << "EEROR   :" << std::endl;
        exit(994);
    }

    void zero() {
#ifdef _DEBUG
        std::cout << "0|zero     : " << this << std::endl;
#endif
        memset(data, 0, count * sizeof(T));
    }

    void p(int i, int j) {
        std::cout << " |v(" << i << "," << j << ") = " << value(i, j) << std::endl;
    }

    void p() {
        std::cout.width(12); std::cout << "|||||||||||||";
        for (int i = 0; i < c; i++) {
            std::cout.width(12); std::cout << i << "|";
        }
        std::cout << std::endl;
        std::cout.width(12); std::cout << "|||||||||||||";
        for (int i = 0; i < c; i++) {
            std::cout.width(12); std::cout << "____________" << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < r; i++) {
            std::cout.width(12); std::cout << i << "|";
            for (int j = 0; j < c; j++) {
                std::cout.width(12); std::cout << data[i + j * r] << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    T& value(int i, int j) {
        // function value:
        //		i: row index
        //		j: column index
        //		return: value of matrix at (i,j)
        if (i < r && j < c) return data[i + r * j];
        std::cout << "EEROR   :" << std::endl;
        exit(999);
    }

    inline bool check(const Vector2D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|check    : " << this << std::endl;
#endif
        if (r != a.r) return false;
        if (c != a.c) return false;
        return true;
    }
};
//====================================================================================================
template <typename T> class Vector3D {
public:
    int r, c, v, count;
    T* data;
    Vector3D(const int r, const int c, const int v) : r(r), c(c), v(v) {
        count = r * c * v;	data = new T[r * c * v];
#ifdef _DEBUG
        std::cout << "+|create   : " << this << std::endl;
#endif
    }

    Vector3D(const Vector3D<T>& a) : r(a.r), c(a.c), v(a.v) {
        count = r * c * v;	data = new T[r * c * v];
        memcpy(data, a.data, count * sizeof(T));
#ifdef _DEBUG
        std::cout << "+|copy     : " << this << std::endl;
#endif
    }
    Vector3D() : r(0), c(0), v(0) {
        count = 0;
        data = nullptr;
#ifdef _DEBUG
        std::cout << "+|create   : " << this << std::endl;
#endif
    }

    ~Vector3D() {
#ifdef _DEBUG
        std::cout << "-|destroy  : " << this << std::endl;
#endif
        if (data) delete[] data;
    }

    void clear() {
        r = 0;
        c = 0;
        v = 0;
        count = 0;
        if (data) delete[] data;
    }


    void set(const int r, const int c) {
        this->r = r;	this->c = c;	this->v = v;
        count = r * c * v;
        if (data) delete[] data;
        data = new T[r * c * v];
#ifdef _DEBUG
        std::cout << "#|set      : " << this << std::endl;
#endif
    }

    void allocate(const int r, const int c) {
        this->r = r;	this->c = c;	this->v = v;
        count = r * c * v;
        if (data) delete[] data;
        data = new T[r * c * v];
#ifdef _DEBUG
        std::cout << "#|set      : " << this << std::endl;
#endif
    }

    void range(Vector3D<T>& a, int istart, int iend, int jstart, int jend, int kstart, int kend) {
        allocate(iend - istart + 1, jend - jstart + 1, kend - kstart + 1);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                for (int k = 0; k < v; k++)
                    value(i, j, k) = a(istart + i, jstart + j, kstart + k);
            }
        }
    }

    Vector3D<T>& operator=(const Vector3D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op =     : " << this << "  " << &a << std::endl;
#endif
        if (check(a)) {
            memcpy(data, a.data, count * sizeof(T));
            return *this;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    void operator=(const T a) {
#ifdef _DEBUG
        std::cout << "#|op =     : " << this << "  " << &a << std::endl;
#endif
        memcpy(data, a.data, count * sizeof(T));
    }

    T& operator()(const int i, const int j, const int k) {
#ifdef _DEBUG
        std::cout << "#|op ()    : " << this << std::endl;
#endif
        if (i < r && j < c)	return data[i + r * j + r * c * k]; //<<==============================
        std::cout << "EEROR   :" << std::endl;
        exit(999);
    }

    Vector3D<T> operator+(const Vector3D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op +     : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            Vector3D<T> b(r, c, v);
            for (int i = 0; i < count; i++) b.data[i] = a.data[i] + data[i];
            return b;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    Vector3D<T> operator*(const Vector3D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op +     : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            Vector3D<T> b(r, c, v);
            for (int i = 0; i < count; i++) b.data[i] = a.data[i] * data[i];
            return b;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    Vector3D<T> operator-(const Vector3D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|op -     : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            Vector3D<T> b(r, c, v);
            for (int i = 0; i < count; i++) b.data[i] = data[i] - a.data[i];
            return b;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(998);
    }

    Vector3D<T> add(const Vector3D<T>& a, const Vector3D<T>& b)
    {
#ifdef _DEBUG
        std::cout << "#|add      : " << this << " " << &a << " " << &b << std::endl;
#endif
        if (check(a) && check(b)) {
            Vector3D<T> cc(r, c, v);
            for (int i = 0; i < count; i++) cc.data[i] = a.data[i] + b.data[i];
            return cc;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(997);
    }

    void add(const Vector3D<T>& a)
    {
#ifdef _DEBUG
        std::cout << "#|add      : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            for (int i = 0; i < count; i++) data[i] += a.data[i];
        }
        std::cout << "EEROR   :" << std::endl;
        exit(996);
    }

    Vector3D<T> subtract(const Vector3D<T>& a, const Vector3D<T>& b)
    {
#ifdef _DEBUG
        std::cout << "#|subtract : " << this << " " << &a << " " << &b << std::endl;
#endif
        if (check(a) && check(b)) {
            Vector3D<T> cc(r, c, v);
            for (int i = 0; i < count; i++) cc.data[i] = a.data[i] - b.data[i];
            return cc;
        }
        std::cout << "EEROR   :" << std::endl;
        exit(995);
    }
    void subtract(const Vector3D<T>& a)
    {
#ifdef _DEBUG
        std::cout << "#|subtract : " << this << " " << &a << std::endl;
#endif
        if (check(a)) {
            for (int i = 0; i < count; i++) data[i] -= a.data[i];
        }
        std::cout << "EEROR   :" << std::endl;
        exit(994);
    }

    void zero() {
#ifdef _DEBUG
        std::cout << "0|zero     : " << this << std::endl;
#endif
        memset(data, 0, count * sizeof(T));
    }

    void p(int i, int j, int k) {
        std::cout << " |v(" << i << "," << j << ") = " << value(i, j, k) << std::endl;
    }

    void p() {

    }

private:
    T& value(int i, int j, int k) {
        if (i < r && j < c && k < v) return data[i + r * j];
        std::cout << "EEROR   :" << std::endl;
        exit(999);
    }

    inline bool check(const Vector3D<T>& a) {
#ifdef _DEBUG
        std::cout << "#|check    : " << this << std::endl;
#endif
        if (r != a.r) return false;
        if (c != a.c) return false;
        if (v != a.v) return false;
        return true;
    }
};
//====================================================================================================
class thread_pool
{
public:
    // ============================
    // Constructors and destructor
    // ============================
    thread_pool(const ui32& _thread_count = std::thread::hardware_concurrency())
        : thread_count(_thread_count ? _thread_count : std::thread::hardware_concurrency()), threads(new std::thread[_thread_count ? _thread_count : std::thread::hardware_concurrency()])
    {
        create_threads();
    }

    ~thread_pool()
    {
        wait_for_tasks();
        running = false;
        destroy_threads();
    }

    // =======================
    // Public member functions
    // =======================

    /**
     * @brief Get the number of tasks currently waiting in the queue to be executed by the threads.
     * @return The number of queued tasks.
     */
    size_t get_tasks_queued() const
    {
        const std::scoped_lock lock(queue_mutex);
        return tasks.size();
    }

    /**
     * @brief Get the number of tasks currently being executed by the threads.
     * @return The number of running tasks.
     */
    size_t get_tasks_running() const
    {
        return tasks_total - get_tasks_queued();
    }

    /**
     * @brief Get the total number of unfinished tasks - either still in the queue, or running in a thread.
     * @return The total number of tasks.
     */
    ui32 get_tasks_total() const
    {
        return tasks_total;
    }

    /**
     * @brief Get the number of threads in the pool.
     * @return The number of threads.
     */
    ui32 get_thread_count() const
    {
        return thread_count;
    }

    /**
     * @brief Parallelize a loop by splitting it into blocks, submitting each block separately to the thread pool, and waiting for all blocks to finish executing. The user supplies a loop function, which will be called once per block and should iterate over the block's range.
     *
     * @tparam T1 The type of the first index in the loop. Should be a signed or unsigned integer.
     * @tparam T2 The type of the index after the last index in the loop. Should be a signed or unsigned integer. If T1 is not the same as T2, a common type will be automatically inferred.
     * @tparam F The type of the function to loop through.
     * @param first_index The first index in the loop.
     * @param index_after_last The index after the last index in the loop. The loop will iterate from first_index to (index_after_last - 1) inclusive. In other words, it will be equivalent to "for (T i = first_index; i < index_after_last; i++)". Note that if first_index == index_after_last, the function will terminate without doing anything.
     * @param loop The function to loop through. Will be called once per block. Should take exactly two arguments: the first index in the block and the index after the last index in the block. loop(start, end) should typically involve a loop of the form "for (T i = start; i < end; i++)".
     * @param num_blocks The maximum number of blocks to split the loop into. The default is to use the number of threads in the pool.
     */
    template <typename T1, typename T2, typename F>
    void parallelize_loop(const T1& first_index, const T2& index_after_last, const F& loop, ui32 num_blocks = 0)
    {
        typedef std::common_type_t<T1, T2> T;
        T the_first_index = (T)first_index;
        T last_index = (T)index_after_last;
        if (the_first_index == last_index)
            return;
        if (last_index < the_first_index)
        {
            T temp = last_index;
            last_index = the_first_index;
            the_first_index = temp;
        }
        last_index--;
        if (num_blocks == 0)
            num_blocks = thread_count;
        ui64 total_size = (ui64)(last_index - the_first_index + 1);
        ui64 block_size = (ui64)(total_size / num_blocks);
        if (block_size == 0)
        {
            block_size = 1;
            num_blocks = (ui32)total_size > 1 ? (ui32)total_size : 1;
        }
        std::atomic<ui32> blocks_running = 0;
        for (ui32 t = 0; t < num_blocks; t++)
        {
            T start = ((T)(t * block_size) + the_first_index);
            T end = (t == num_blocks - 1) ? last_index + 1 : ((T)((t + 1) * block_size) + the_first_index);
            blocks_running++;
            push_task([start, end, &loop, &blocks_running]
                {
                    loop(start, end);
                    blocks_running--;
                });
        }
        while (blocks_running != 0)
        {
            sleep_or_yield();
        }
    }

    /**
     * @brief Push a function with no arguments or return value into the task queue.
     * @tparam F The type of the function.
     * @param task The function to push.
     */
    template <typename F>
    void push_task(const F& task)
    {
        tasks_total++;
        {
            const std::scoped_lock lock(queue_mutex);
            tasks.push(std::function<void()>(task));
        }
    }

    /**
     * @brief Push a function with arguments, but no return value, into the task queue.
     * @details The function is wrapped inside a lambda in order to hide the arguments, as the tasks in the queue must be of type std::function<void()>, so they cannot have any arguments or return value. If no arguments are provided, the other overload will be used, in order to avoid the (slight) overhead of using a lambda.
     *
     * @tparam F The type of the function.
     * @tparam A The types of the arguments.
     * @param task The function to push.
     * @param args The arguments to pass to the function.
     */
    template <typename F, typename... A>
    void push_task(const F& task, const A &...args)
    {
        push_task([task, args...]
            { task(args...); });
    }

    /**
     * @brief Reset the number of threads in the pool. Waits for all currently running tasks to be completed, then destroys all threads in the pool and creates a new thread pool with the new number of threads. Any tasks that were waiting in the queue before the pool was reset will then be executed by the new threads. If the pool was paused before resetting it, the new pool will be paused as well.
     *
     * @param _thread_count The number of threads to use. The default value is the total number of hardware threads available, as reported by the implementation. With a hyperthreaded CPU, this will be twice the number of CPU cores. If the argument is zero, the default value will be used instead.
     */
    void reset(const ui32& _thread_count = std::thread::hardware_concurrency())
    {
        bool was_paused = paused;
        paused = true;
        wait_for_tasks();
        running = false;
        destroy_threads();
        thread_count = _thread_count ? _thread_count : std::thread::hardware_concurrency();
        threads.reset(new std::thread[thread_count]);
        paused = was_paused;
        running = true;
        create_threads();
    }

    /**
     * @brief Submit a function with zero or more arguments and no return value into the task queue, and get an std::future<bool> that will be set to true upon completion of the task.
     *
     * @tparam F The type of the function.
     * @tparam A The types of the zero or more arguments to pass to the function.
     * @param task The function to submit.
     * @param args The zero or more arguments to pass to the function.
     * @return A future to be used later to check if the function has finished its execution.
     */
    template <typename F, typename... A, typename = std::enable_if_t<std::is_void_v<std::invoke_result_t<std::decay_t<F>, std::decay_t<A>...>>>>
    std::future<bool> submit(const F& task, const A &...args)
    {
        std::shared_ptr<std::promise<bool>> task_promise(new std::promise<bool>);
        std::future<bool> future = task_promise->get_future();
        push_task([task, args..., task_promise]
            {
                try
                {
                    task(args...);
                    task_promise->set_value(true);
                }
                catch (...)
                {
                    try
                    {
                        task_promise->set_exception(std::current_exception());
                    }
                    catch (...)
                    {
                    }
                }
            });
        return future;
    }

    /**
     * @brief Submit a function with zero or more arguments and a return value into the task queue, and get a future for its eventual returned value.
     *
     * @tparam F The type of the function.
     * @tparam A The types of the zero or more arguments to pass to the function.
     * @tparam R The return type of the function.
     * @param task The function to submit.
     * @param args The zero or more arguments to pass to the function.
     * @return A future to be used later to obtain the function's returned value, waiting for it to finish its execution if needed.
     */
    template <typename F, typename... A, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<A>...>, typename = std::enable_if_t<!std::is_void_v<R>>>
    std::future<R> submit(const F& task, const A &...args)
    {
        std::shared_ptr<std::promise<R>> task_promise(new std::promise<R>);
        std::future<R> future = task_promise->get_future();
        push_task([task, args..., task_promise]
            {
                try
                {
                    task_promise->set_value(task(args...));
                }
                catch (...)
                {
                    try
                    {
                        task_promise->set_exception(std::current_exception());
                    }
                    catch (...)
                    {
                    }
                }
            });
        return future;
    }

    /**
     * @brief Wait for tasks to be completed. Normally, this function waits for all tasks, both those that are currently running in the threads and those that are still waiting in the queue. However, if the variable paused is set to true, this function only waits for the currently running tasks (otherwise it would wait forever). To wait for a specific task, use submit() instead, and call the wait() member function of the generated future.
     */
    void wait_for_tasks()
    {
        while (true)
        {
            if (!paused)
            {
                if (tasks_total == 0)
                    break;
            }
            else
            {
                if (get_tasks_running() == 0)
                    break;
            }
            sleep_or_yield();
        }
    }

    // ===========
    // Public data
    // ===========

    /**
     * @brief An atomic variable indicating to the workers to pause. When set to true, the workers temporarily stop popping new tasks out of the queue, although any tasks already executed will keep running until they are done. Set to false again to resume popping tasks.
     */
    std::atomic<bool> paused = false;

    /**
     * @brief The duration, in microseconds, that the worker function should sleep for when it cannot find any tasks in the queue. If set to 0, then instead of sleeping, the worker function will execute std::this_thread::yield() if there are no tasks in the queue. The default value is 1000.
     */
    ui32 sleep_duration = 0;

private:
    // ========================
    // Private member functions
    // ========================

    /**
     * @brief Create the threads in the pool and assign a worker to each thread.
     */
    void create_threads()
    {
        for (ui32 i = 0; i < thread_count; i++)
        {
            threads[i] = std::thread(&thread_pool::worker, this);
        }
    }

    /**
     * @brief Destroy the threads in the pool by joining them.
     */
    void destroy_threads()
    {
        for (ui32 i = 0; i < thread_count; i++)
        {
            threads[i].join();
        }
    }

    /**
     * @brief Try to pop a new task out of the queue.
     *
     * @param task A reference to the task. Will be populated with a function if the queue is not empty.
     * @return true if a task was found, false if the queue is empty.
     */
    bool pop_task(std::function<void()>& task)
    {
        const std::scoped_lock lock(queue_mutex);
        if (tasks.empty())
            return false;
        else
        {
            task = std::move(tasks.front());
            tasks.pop();
            return true;
        }
    }

    /**
     * @brief Sleep for sleep_duration microseconds. If that variable is set to zero, yield instead.
     *
     */
    void sleep_or_yield()
    {
        if (sleep_duration)
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_duration));
        else
            std::this_thread::yield();
    }

    /**
     * @brief A worker function to be assigned to each thread in the pool. Continuously pops tasks out of the queue and executes them, as long as the atomic variable running is set to true.
     */
    void worker()
    {
        while (running)
        {
            std::function<void()> task;
            if (!paused && pop_task(task))
            {
                task();
                tasks_total--;
            }
            else
            {
                sleep_or_yield();
            }
        }
    }

    // ============
    // Private data
    // ============

    /**
     * @brief A mutex to synchronize access to the task queue by different threads.
     */
    mutable std::mutex queue_mutex = {};

    /**
     * @brief An atomic variable indicating to the workers to keep running. When set to false, the workers permanently stop working.
     */
    std::atomic<bool> running = true;

    /**
     * @brief A queue of tasks to be executed by the threads.
     */
    std::queue<std::function<void()>> tasks = {};

    /**
     * @brief The number of threads in the pool.
     */
    ui32 thread_count;

    /**
     * @brief A smart pointer to manage the memory allocated for the threads.
     */
    std::unique_ptr<std::thread[]> threads;

    /**
     * @brief An atomic variable to keep track of the total number of unfinished tasks - either still in the queue, or running in a thread.
     */
    std::atomic<ui32> tasks_total = 0;
};
//====================================================================================================
class synced_stream
{
public:
    /**
     * @brief Construct a new synced stream.
     *
     * @param _out_stream The output stream to print to. The default value is std::cout.
     */
    synced_stream(std::ostream& _out_stream = std::cout)
        : out_stream(_out_stream) {};

    /**
     * @brief Print any number of items into the output stream. Ensures that no other threads print to this stream simultaneously, as long as they all exclusively use this synced_stream object to print.
     *
     * @tparam T The types of the items
     * @param items The items to print.
     */
    template <typename... T>
    void print(const T &...items)
    {
        const std::scoped_lock lock(stream_mutex);
        (out_stream << ... << items);
    }

    /**
     * @brief Print any number of items into the output stream, followed by a newline character. Ensures that no other threads print to this stream simultaneously, as long as they all exclusively use this synced_stream object to print.
     *
     * @tparam T The types of the items
     * @param items The items to print.
     */
    template <typename... T>
    void println(const T &...items)
    {
        print(items..., '\n');
    }

private:
    /**
     * @brief A mutex to synchronize printing.
     */
    mutable std::mutex stream_mutex = {};

    /**
     * @brief The output stream to print to.
     */
    std::ostream& out_stream;
};
//====================================================================================================
inline int      clampI(int v, int st, int en) { if(v < st) return st; if(v > en) return en; return v; }
inline float    clampF(float v, float st, float en) { if(v < st) return st; if(v > en) return en; return v; }
inline int      clampIcycle(int v, int st, int en) { if(v < st) return clampIcycle(st - v, st, en); if(v > en) return clampIcycle(v - en, st, en); return v; }
//====================================================================================================
inline void     HLog() { std::cout << std::endl; };
//====================================================================================================
inline std::string ltrim(const std::string& s) { size_t start = s.find_first_not_of(WHITESPACE); return (start == std::string::npos) ? "" : s.substr(start); }
//------
inline std::string rtrim(const std::string& s) { size_t end = s.find_last_not_of(WHITESPACE); return (end == std::string::npos) ? "" : s.substr(0, end + 1); }
//------
inline std::string trim(const std::string& s) { return rtrim(ltrim(s)); }
//====================================================================================================
inline std::vector<std::string> split(const char* str, char separator)
{
	std::vector<std::string> result;
	do
	{
		const char* begin = str;
		while(*str != separator && *str)
			str++;
		result.push_back(std::string(begin, str));
	} while(0 != *str++);
	return result;
}
//====================================================================================================
inline void        pwd() { std::filesystem::path cwd = std::filesystem::current_path(); println(cwd.string()); }
//------
inline std::string getCurrentDirectory() { std::filesystem::path cwd = std::filesystem::current_path(); return cwd.string(); }
//------
inline std::string getParentDirectory() {
	return (fs::current_path().fs::path::parent_path()).string();
}
//====================================================================================================
inline std::string str2upper(std::string s) { std::transform(s.begin(), s.end(), s.begin(), ::toupper); return s; }
inline std::string str2lower(std::string s) { std::transform(s.begin(), s.end(), s.begin(), ::tolower); return s; }
//====================================================================================================
inline bool strCompare(std::string v1, std::string v2, bool strict) {
	if(!strict)
		if(str2lower(trim(v1)) == str2lower(trim(v2)))
			return true;
		else
			if(trim(v1) == trim(v2))
				return true;
	return false;
}
//====================================================================================================
inline void RedirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE* fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long) GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();
}
//====================================================================================================

//char c = 120;
//auto s0 = std::format("{:6}", 42);    // value of s0 is "    42"
//auto s1 = std::format("{:6}", 'x');   // value of s1 is "x     "
//auto s2 = std::format("{:*<6}", 'x'); // value of s2 is "x*****"
//auto s3 = std::format("{:*>6}", 'x'); // value of s3 is "*****x"
//auto s4 = std::format("{:*^6}", 'x'); // value of s4 is "**x***"
//auto s5 = std::format("{:6d}", c);    // value of s5 is "   120"
//auto s6 = std::format("{:6}", true);  // value of s6 is "true  "
//double inf = std::numeric_limits<double>::infinity();
//double nan = std::numeric_limits<double>::quiet_NaN();
//auto s0 = std::format("{0:},{0:+},{0:-},{0: }", 1);   // value of s0 is "1,+1,1, 1"
//auto s1 = std::format("{0:},{0:+},{0:-},{0: }", -1);  // value of s1 is "-1,-1,-1,-1"
//auto s2 = std::format("{0:},{0:+},{0:-},{0: }", inf); // value of s2 is "inf,+inf,inf, inf"
//auto s3 = std::format("{0:},{0:+},{0:-},{0: }", nan); // value of s3 is "nan,+nan,nan, nan"
//char c = 120;
//auto s1 = std::format("{:+06d}", c);   // value of s1 is "+00120"
//auto s2 = std::format("{:#06x}", 0xa); // value of s2 is "0x000a"
//auto s3 = std::format("{:<06}", -42);  // value of s3 is "-42   "
//float pi = 3.14f;
//auto s1 = std::format("{:10f}", pi);              // s1 = "  3.140000" (width = 10)
//auto s2 = std::format("{:{}f}", pi, 10);          // s2 = "  3.140000" (width = 10)
//auto s3 = std::format("{:.5f}", pi);              // s3 = "3.14000" (precision = 5)
//auto s4 = std::format("{:.{}f}", pi, 5);          // s4 = "3.14000" (precision = 5)
//auto s5 = std::format("{:10.5f}", pi);            // s5 = "   3.14000"
//											        // (width = 10, precision = 5)
//auto s6 = std::format("{:{}.{}f}", pi, 10, 5);    // s6 = "   3.14000"
//											        // (width = 10, precision = 5)
//auto s1 = std::format("{:.^5s}", "🐱");           // s1 = ".🐱.."
//auto s2 = std::format("{:.5s}", "🐱🐱🐱");       // s2 = "🐱🐱"
//auto s3 = std::format("{:.<5.5s}", "🐱🐱🐱");    // s3 = "🐱🐱."
//L(locale - specific formatting)
//	The L option causes the locale - specific form to be used.This option is only valid for arithmetic types.
//
//	For integral types, the locale - specific form inserts the appropriate digit group separator characters according to 
//  the context's locale.
//	For floating - point types, the locale - specific form inserts the appropriate digit group and radix separator characters 
//  according to the context's locale.
//	For the textual representation of bool, the locale - specific form uses the appropriate string as if obtained with 
//  std::numpunct::truename or std::numpunct::falsename. type
//	The available string presentation types are :
//
//none, s : Copies the string to the output.
//	The available integer presentation types for integral types other than char, wchar_t, and bool are :
//
//b: Binary format.Produces the output as if by calling std::to_chars(first, last, value, 2).The base prefix is 0b.
//B: same as b, except that the base prefix is 0B.
//c: Copies the character static_cast<CharT>(value) to the output, where CharT is the character type of the format string.
//   Throws std::format_error if value is not in the range of representable values for CharT.
//d : Decimal format.Produces the output as if by calling std::to_chars(first, last, value).
//o : Octal format.Produces the output as if by calling std::to_chars(first, last, value, 8).
//    The base prefix is 0 if the corresponding argument value is nonzeroand is empty otherwise.
//x : Hex format.Produces the output as if by calling std::to_chars(first, last, value, 16).
//    The base prefix is 0x.
//X: same as x, except that it uses uppercase letters for digits above 9 and the base prefix is 0X.
//none: same as d.
// 
//The available charand wchar_t presentation types are :
//
//none, c : Copies the character to the output.
//b, B, d, o, x, X : Uses integer presentation types.
//The available bool presentation types are :
//
//none, s : Copies textual representation(true or false, or the locale - specific form) to the output.
//b, B, d, o, x, X : Uses integer presentation types with the value static_cast<unsigned char>(value).
//The available floating - point presentation types are :
//
//a: If precision is specified, produces the output as if by calling 
//   std::to_chars(first, last, value, std::chars_format::hex, precision) where precision is the specified precision; 
//   otherwise, the output is produced as if by calling std::to_chars(first, last, value, std::chars_format::hex).
//A: same as a, except that it uses uppercase letters for digits above 9 and uses P to indicate the exponent.
//e : Produces the output as if by calling std::to_chars(first, last, value, std::chars_format::scientific, precision) 
//    where precision is the specified precision, or 6 if precision is not specified.
//E : same as e, except that it uses E to indicate the exponent.
//f, F : Produces the output as if by calling std::to_chars(first, last, value, std::chars_format::fixed, precision) 
//    where precision is the specified precision, or 6 if precision is not specified.
//g : Produces the output as if by calling std::to_chars(first, last, value, std::chars_format::general, precision) 
//    where precision is the specified precision, or 6 if precision is not specified.
//G : same as g, except that it uses E to indicate the exponent.
//none : If precision is specified, produces the output as if by calling 
//       std::to_chars(first, last, value, std::chars_format::general, precision) 
//       where precision is the specified precision; otherwise, the output is produced as if by calling std::to_chars(first, last, value).
//For lower - case presentation types, infinityand NaN are formatted as infand nan, respectively.
//For upper - case presentation types, infinityand NaN are formatted as INFand NAN, respectively.
//
//The available pointer presentation types(also used for std::nullptr_t) are:
//
//	none, p : If std::uintptr_t is defined, produces the output as if by calling 
// std::to_chars(first, last, reinterpret_cast<std::uintptr_t>(value), 16) with the prefix 0x 
// added to the output; otherwise, the output is implementation - defined.


