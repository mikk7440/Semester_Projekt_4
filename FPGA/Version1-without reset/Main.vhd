
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
		Hallindex1 : IN STD_LOGIC;
		Hallindex2 : IN STD_LOGIC;
		Led  	: OUT STD_LOGIC_VECTOR(7 downto 0);
		Enc1 	: IN STD_LOGIC_VECTOR(1 downto 0);
		Enc2 	: IN STD_LOGIC_VECTOR(1 downto 0);		
		Segm 	: OUT STD_LOGIC_VECTOR(7 downto 0);
		An	  	: OUT STD_LOGIC_VECTOR(3 downto 0);
		PWMR1  : OUT STD_LOGIC;
		PWML1  : OUT STD_LOGIC;
		PWMR2  : OUT STD_LOGIC;
		PWML2  : OUT STD_LOGIC;
		En1		: OUT STD_LOGIC;
		En2		: OUT STD_LOGIC
);
end Main;

architecture Behavioral of Main is
component MotorController
    port(
-------------Default---------------
		CLK  		:	IN STD_LOGIC;
		Reset		: In STD_LOGIC;
--------------End------------------
------------Controller-------------
		Enable	:	OUT	STD_LOGIC;
		Right		:	OUT	STD_LOGIC;
		Left		:	OUT	STD_LOGIC;
--------------End------------------
--------------PWM------------------
		DutyCycle:	IN	 	STD_LOGIC_VECTOR(7 downto 0);
--------------End------------------
 
------------Encoder----------------
		EncoderIn: 	IN STD_LOGIC_VECTOR(1 downto 0);
		EncoderOut:	OUT STD_LOGIC_VECTOR(13 downto 0);
		VelocityOut  : OUT STD_LOGIC_VECTOR(8 downto 0);
--------------End------------------

		HallIndex : IN STD_LOGIC

		);
end component;
component ProtocolSpi
	Port(
		CLK	: in STD_LOGIC;
		Encoder1 : in STD_LOGIC_VECTOR(13 downto 0);
		Encoder2 : in STD_LOGIC_VECTOR(13 downto 0);
		Velocity1	: in STD_LOGIC_VECTOR(8 downto 0);
		Velocity2	: in STD_LOGIC_VECTOR(8 downto 0);
		Motor1Duty : out STD_LOGIC_VECTOR(7 downto 0);
		Motor2Duty : out STD_LOGIC_VECTOR(7 downto 0);		
		SCLK : IN STD_LOGIC;
		MOSI : IN STD_LOGIC;
		SS	  : IN STD_LOGIC;
		MISO : OUT STD_LOGIC;
		Led  : OUT STD_LOGIC_VECTOR(7 downto 0);
		HomePos : OUT STD_LOGIC;
		Seg7Motor1	: out STD_LOGIC;
		Seg7Motor2	: out STD_LOGIC
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

signal BindingReset : STD_LOGIC;
signal Test_Encoder1: STD_LOGIC_VECTOR(13 downto 0);
signal Test_Encoder2: STD_LOGIC_VECTOR(13 downto 0);
signal Test_Duty1 	: STD_LOGIC_VECTOR(7 downto 0);
signal Test_Duty2 	: STD_LOGIC_VECTOR(7 downto 0);
signal Velocity1		: STD_LOGIC_VECTOR(8 downto 0);
signal Velocity2		: STD_LOGIC_VECTOR(8 downto 0);
signal seg7Number		: STD_LOGIC_VECTOR (13 downto 0);
signal Seg7Motor1		: STD_LOGIC;
signal Seg7Motor2		: STD_LOGIC;


begin
--Led <= Test_Duty2;

U1 : ProtocolSpi PORT MAP(CLK=>CLK,
						Encoder1=>Test_Encoder1,
						Encoder2=>Test_Encoder2,
						Velocity1=>Velocity1,
						Velocity2=>Velocity2,
						Motor1Duty=>Test_Duty1,
						Motor2Duty=>Test_Duty2,
						SCLK=>SCLK,
						MOSI=>MOSI,
						SS=>SS,
						MISO=>MISO,
						HomePos=>BindingReset,
						Seg7Motor1=> Seg7Motor1,
						Seg7Motor2=>Seg7Motor2,
						Led => Led
						);
			
U2 : MotorController PORT MAP(
						CLK=>CLK,
						Enable => En1,
						Right => PWMR1,
						Left => PWML1,
						DutyCycle => Test_Duty1,
						EncoderIn => Enc1,		
						EncoderOut => Test_Encoder1,
						Reset=>BindingReset,
						VelocityOut => Velocity1,
						HallIndex => Hallindex1
						);
						
U3 : MotorController PORT MAP(
						CLK=>CLK,
						Enable => En2,
						Right => PWMR2,
						Left => PWML2,
						DutyCycle => Test_Duty2,
						EncoderIn => Enc2,
						EncoderOut => Test_Encoder2,
						Reset=>BindingReset,
						VelocityOut => Velocity2,
						HallIndex => Hallindex2
						);
						
U4 : seg7 PORT MAP(
						CLK=>CLK,
						number=>seg7Number,
						Segm=>Segm,
						An=>An
						);
						


process(CLK)
begin
	if rising_edge(CLK) then
		if Seg7Motor1 = '1' then
			seg7Number <= "00000" & Velocity1;				--"00000" & Velocity1;
		end if;
		
		if Seg7Motor2 = '1' then
			seg7Number <= "00000" & Velocity2;
		end if;
	end if;
end process;

--Led <= Hallindex1 & "000000" & Hallindex2;
end Behavioral;


