#include "dht11.h"
#include <stdio.h>

DHT11::DHT11() :
_TEST_COUNT(0)
{

}

int DHT11::Update()
{
	_TEST_COUNT += 20;
	return _TEST_COUNT;
}
