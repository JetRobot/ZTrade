#ifndef types_h
#define types_h

//TN is for Terrre-Neuve
//define
#define TN_INTEGER int
#define TN_LONG_INTEGER long
#define TN_REAL double
#define TN_INFINITY 9999

//custom types
typedef short TN_INTEGER ShortInteger;
typedef unsigned short TN_INTEGER ShortNatural;
typedef TN_INTEGER Integer;
typedef unsigned TN_INTEGER Natural;
typedef TN_LONG_INTEGER LongInteger;
typedef unsigned TN_LONG_INTEGER LongNatural;
typedef TN_REAL Real;
typedef long long VeryLongInteger;
typedef unsigned long long VeryLongNatural;

enum Currency { USD = 1,
				EUR = 2,
				CAD = 3 };


struct cachedval {
	bool isCached;
	Real value;
};


#endif