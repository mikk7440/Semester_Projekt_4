
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Main is
port( 
		CLK  	: in STD_LOGIC;
		SCLK 	: IN STD_LOGIC;
		MOSI 	: IN STD_LOGIC;
		SS	  	: IN STD_LOGIC;
		MISO 	: OUT STD_LOGIC;
		Led  	: OUT STD_LOGIC_VECTOR(7 downto 0);
		Enc 	: IN STD_LOGIC_VECTOR(1 downto 0);
		Segm 	: OUT STD_LOGIC_VECTOR(7 downto 0);
		An	  	: OUT STD_LOGIC_VECTOR(3 downto 0);
		PWMR  : OUT STD_LOGIC;
		PWML  : OUT STD_LOGIC;
		En		: OUT STD_LOGIC
);
end Main;

architecture Behavioral of Main is
component SPI
    port(
		CLK  : in STD_LOGIC;
		SCLK : IN STD_LOGIC;
		MOSI : IN STD_LOGIC;
		SS	  : IN STD_LOGIC;
		TX	  : IN STD_LOGIC_VECTOR(15 downto 0);
		RX	  : OUT STD_LOGIC_VECTOR(15 downto 0);
		MISO : OUT STD_LOGIC
		);
end component;
component PWM
    port(
	CLK : IN STD_LOGIC;
	DutyCycleIN : IN STD_LOGIC_VECTOR (7 downto 0);
	PWMRIGHTOUT	: OUT STD_LOGIC;
	PWMLEFTTOUT	: OUT STD_LOGIC
	);
end component;

component Encoder
port (
		Reset : in STD_LOGIC;
		CLK : in STD_LOGIC;
		HallSensor : in STD_LOGIC_VECTOR (1 downto 0); --lilla (0) and blaa (1)
		EncoderOut : out STD_LOGIC_VECTOR(13 downto 0)
		);
end component;

component seg7
Port(
	CLK : in std_logic;
	number : in STD_LOGIC_VECTOR (13 downto 0);
	Segm : out  STD_LOGIC_VECTOR (7 downto 0);
	An :   out  STD_LOGIC_VECTOR (3 downto 0)
	);
end component;

signal Test_Duty 	: STD_LOGIC_VECTOR(7 downto 0);
signal Test_TX 	: STD_LOGIC_VECTOR(15 downto 0);
signal Test_RX 	: STD_LOGIC_VECTOR(15 downto 0);
signal LED_TEST1 	: STD_LOGIC;
signal LED_TEST2 	: STD_LOGIC;
signal LED_TEST3 	: STD_LOGIC_VECTOR(13 downto 0);

begin
U1 : SPI PORT MAP(CLK=>CLK,
						SCLK=>SCLK,
						MOSI=>MOSI,
						SS=>SS,
						MISO=>MISO,
						TX=>Test_TX,
						RX=>Test_RX
						);
U2	: PWM PORT MAP(CLK=>CLK,
						DutyCycleIN=>Test_Duty,
						PWMLEFTTOUT=>LED_TEST1,
						PWMRIGHTOUT=>LED_TEST2
						);
						
U3	: Encoder PORT MAP(
						CLK=>CLK,
						HallSensor=>Enc,
						EncoderOut=>LED_TEST3,
						Reset=>LED_TEST2
						);
						
						
U4 : seg7 PORT MAP(
						CLK=>CLK,
						number=>LED_TEST3,
						Segm=>Segm,
						An=>An
						);

--Test_Duty<=Test_RX(7 downto 0);
--Led(4 downto 0) <= (others => LED_TEST1);
--Led(7 downto 5) <= (others => LED_TEST2);
--Led<= Test_RX(7 downto 0);
Test_Duty<=Test_RX(7 downto 0);
Test_TX <= "00"&LED_TEST3;
PWMR<= LED_TEST1;
PWML<= LED_TEST2;
En<= '1';
end Behavioral;


