
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Main is
port( 
		CLK  	: in STD_LOGIC;
		Sw		: IN STD_LOGIC_VECTOR(7 downto 0);
		SSCLK 	: IN STD_LOGIC;
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
		En2		: OUT STD_LOGIC;
		CS			: out STD_LOGIC;
		D0			: in STD_LOGIC;
		D1			: in STD_LOGIC;
		MSclk		: out STD_LOGIC
		
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
		Limit1	: IN STD_LOGIC_VECTOR(13 downto 0);
		Limit2	: IN STD_LOGIC_VECTOR(13 downto 0);
--------------End------------------
 
------------Encoder----------------
		EncoderIn: 	IN STD_LOGIC_VECTOR(1 downto 0);
		EncoderOut:	OUT STD_LOGIC_VECTOR(11 downto 0);
		VelocityOut  : OUT STD_LOGIC_VECTOR(11 downto 0);
--------------End------------------

		HallIndex : IN STD_LOGIC

		);
end component;
component ProtocolSpi
	Port(
		CLK	: in STD_LOGIC;
		Encoder1 : in STD_LOGIC_VECTOR(11 downto 0);
		Encoder2 : in STD_LOGIC_VECTOR(11 downto 0);
		Velocity1	: in STD_LOGIC_VECTOR(11 downto 0);
		Velocity2	: in STD_LOGIC_VECTOR(11 downto 0);
		Motor1Duty : out STD_LOGIC_VECTOR(7 downto 0);
		Motor2Duty : out STD_LOGIC_VECTOR(7 downto 0);	
		Current1 	: in STD_LOGIC_VECTOR(11 downto 0);
		Current2		: in STD_LOGIC_VECTOR(11 downto 0);
		protocolbits: out STD_LOGIC_VECTOR(2 downto 0);
		SSCLK : IN STD_LOGIC;
		MOSI : IN STD_LOGIC;
		SS	  : IN STD_LOGIC;
		MISO : OUT STD_LOGIC;
		--Led  : OUT STD_LOGIC_VECTOR(7 downto 0);
		HomePos : OUT STD_LOGIC
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

component CurrentSensor

		port
		(
			clk_50mhz	:	in 	std_logic;
	--		btn0			:	in		std_logic;
	--		led			:	OUT	std_logic_vector (7 downto 0);
			MSclk 		: 	OUT 	std_logic;
			CS				:	OUT 	std_logic;
			D0				:	in		std_logic;
			D1				:	in		std_logic;
			current1		: out 	STD_LOGIC_VECTOR(11 downto 0);
			current2		: out		STD_LOGIC_VECTOR(11 downto 0)
		);
end component;

signal BindingReset : STD_LOGIC;
signal Test_Encoder1: STD_LOGIC_VECTOR(11 downto 0);
signal Test_Encoder2: STD_LOGIC_VECTOR(11 downto 0);
signal Test_Duty1 	: STD_LOGIC_VECTOR(7 downto 0);
signal Test_Duty2 	: STD_LOGIC_VECTOR(7 downto 0);
signal Velocity1		: STD_LOGIC_VECTOR(11 downto 0);
signal Velocity2		: STD_LOGIC_VECTOR(11 downto 0);
signal Current1		: STD_LOGIC_VECTOR(11 downto 0);
signal Current2		: STD_LOGIC_VECTOR(11 downto 0);
signal seg7Number		: STD_LOGIC_VECTOR (13 downto 0);
signal Seg7Motor1		: STD_LOGIC;
signal Seg7Motor2		: STD_LOGIC;
constant LimitM11		: STD_LOGIC_VECTOR(13 downto 0) :="11111111111111";
constant LimitM12		: STD_LOGIC_VECTOR(13 downto 0) :="00000000000000";
constant LimitM21		: STD_LOGIC_VECTOR(13 downto 0) :="00100010110001";
constant LimitM22		: STD_LOGIC_VECTOR(13 downto 0) :="00011011101111";



signal protocolBits : STD_LOGIC_VECTOR(2 downto 0);


signal MaulalDuty		: STD_LOGIC_VECTOR(7 downto 0);

begin
--Led <= Test_Duty2;


U1 : ProtocolSpi PORT MAP(CLK=>CLK,
						Encoder1=>Test_Encoder1,
						Encoder2=>Test_Encoder2,
						Velocity1=>Velocity1,
						Velocity2=>Velocity2,
						Motor1Duty=>Test_Duty1,
						Motor2Duty=>Test_Duty2,
						Current1 => Current1,
						Current2 => Current2,
						protocolbits=> protocolbits,
						SSCLK=>SSCLK,
						MOSI=>MOSI,
						SS=>SS,
						MISO=>MISO,
						HomePos=>BindingReset
						--Led => Led
						);
			
U2 : MotorController PORT MAP(
						CLK=>CLK,
						Enable => En1,
						Right => PWMR1,
						Left => PWML1,
						DutyCycle => Test_Duty1, --MaulalDuty,--
						EncoderIn => Enc1,		
						EncoderOut => Test_Encoder1,
						Reset=>BindingReset,
						VelocityOut => Velocity1,
						HallIndex => Hallindex1,
						Limit1=> LimitM11,
						Limit2 => LimitM12
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
						HallIndex => Hallindex2,
						Limit1=> LimitM21,
						Limit2 =>LimitM22
						);
						

	
U4 : CurrentSensor port map(
						clk_50mhz=>CLK,
						MSclk => MSclk,
						CS =>CS,
						D0 =>D0,
						D1 =>D1,
						Current1 => Current1,
						Current2 => Current2
						);	

						
U5 : seg7 PORT MAP(
						CLK=>CLK,
						number=>seg7Number,
						Segm=>Segm,
						An=>An
						);
						


process(CLK)
begin
	if rising_edge(CLK) then
		case Sw is
		when "00000001" => seg7Number <= "00" & Test_Encoder1;
		when "00000010" => seg7Number <= "00" & Test_Encoder2;
		when "00000100" => seg7Number <= "00" & Velocity1;
		when "00001000" => seg7Number	<= "00" & Velocity2;
		when "00010000" => seg7Number <= "00" & Current1;
		when "00100000" => seg7Number <= "00" & Current2;
		when "01000000" => seg7Number <= "000000"& Test_Duty1;
		when "10000000" => seg7Number <= "000000"& Test_Duty2;
		when others => MaulalDuty <=(others =>'0');--null;
		end case;
	end if;
end process;

Led <= Hallindex1 &"0" & Hallindex2& "00"& protocolbits;
end Behavioral;


