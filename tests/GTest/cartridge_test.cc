#include "Cartridge.cpp"
#include "gtest/gtest.h"

TEST(cartridge_test, cartridge_present__test)
{
	// Check with both absent on startup
	EXPECT_EQ(Cartridge__Present(FFF_INDEX),false);
	EXPECT_EQ(Cartridge__Present(SILVER_INDEX),false);

}

TEST(cartridge_test, cartridge_present_invalid_cartridge_test)
{
	// Check with out of bounds
	EXPECT_EQ(Cartridge__Present(3),false);
	EXPECT_EQ(Cartridge__Present(100),false);
}

TEST(cartridge_test, cartridge_update_test)
{
	// Cartridge zero is pulled low by default
	// Cartridge one is pulled high by default
	CART0_SIG2_PIN = LOW;
	CART1_SIG2_PIN = HIGH;
	Cartridge__Update();
	EXPECT_EQ(Cartridge__Present(FFF_INDEX),false);
	EXPECT_EQ(Cartridge__Present(SILVER_INDEX),false);

	CART0_SIG2_PIN = HIGH;
	CART1_SIG2_PIN = LOW;
	Cartridge__Update();
	EXPECT_EQ(Cartridge__Present(FFF_INDEX),true);
	EXPECT_EQ(Cartridge__Present(SILVER_INDEX),true);
}


TEST(cartridge_test, cartridge_fff_not_present_test)
{
	CART0_SIG2_PIN = HIGH;
	CART1_SIG2_PIN = HIGH;
	Cartridge__Update();
	EXPECT_EQ(Cartridge__FFFNotPresent() , false);
	
	CART0_SIG2_PIN = LOW;
	CART1_SIG2_PIN = HIGH;
	Cartridge__Update();
	EXPECT_EQ(Cartridge__FFFNotPresent() , true);

	CART1_SIG2_PIN = LOW;
	Cartridge__Update();
	EXPECT_EQ(Cartridge__FFFNotPresent() , true);
}

TEST(cartridge_test, cartridge_fff_not_present__hysteresis_test_once)
{
	CART0_SIG2_PIN = LOW;
	Cartridge__Update();
	EXPECT_EQ(Cartridge__FFFNotPresentHysteresis() , true);
	CART0_SIG2_PIN = HIGH;
	Cartridge__Update();
	EXPECT_EQ(Cartridge__FFFNotPresentHysteresis() , true);
}

TEST(cartridge_test, cartridge_fff_not_present__hysteresis_test_hysteresis_count)
{
	CART0_SIG2_PIN = LOW;
	Cartridge__Update();
	EXPECT_EQ(Cartridge__FFFNotPresentHysteresis() , true);
	CART0_SIG2_PIN = HIGH;
	// Minus two to see the edge: Called once already (-1), called once after 
	// to get to the last one (-2), then the next one tips us over
	for (int i = 0 ; i < CARTRIDGE_REMOVAL_HYSTERESIS_COUNT - 2 ; i++ ){
		Cartridge__FFFNotPresentHysteresis();
    }
	EXPECT_EQ(Cartridge__FFFNotPresentHysteresis() , true);
	EXPECT_EQ(Cartridge__FFFNotPresentHysteresis() , false);
}

TEST(cartridge_test, cartridge_enable_test) {
	EXPECT_EQ(Cartridge__GetPresentCheck() , true);
	Cartridge__SetPresentCheck(false);
	EXPECT_EQ(Cartridge__GetPresentCheck() , false);
	EXPECT_EQ(Cartridge__FFFNotPresent() , false);
	EXPECT_EQ(Cartridge__FFFNotPresentHysteresis(),false);
	EXPECT_EQ(Cartridge__Present(FFF_INDEX),true);
	EXPECT_EQ(Cartridge__Present(SILVER_INDEX),true);
}
