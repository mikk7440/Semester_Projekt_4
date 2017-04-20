
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity MotorController is
port( 
-------------Default---------------
		CLK  		:	IN STD_LOGIC;
--		Led  		: 	OUT STD_LOGIC_VECTOR(7 downto 0);
--------------End------------------
--------------7Seg-----------------
		Segm 		: 	OUT STD_LOGIC_VECTOR(7 downto 0);
		An	  		: 	OUT STD_LOGIC_VECTOR(3 downto 0);
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
		Reset		: In STD_LOGIC
--------------End------------------

);


end MotorController;

architecture Behavioral of MotorController is
signal PastDutyCycle	: STD_LOGIC_VECTOR(7 downto 0);
signal TempEncouderOut: STD_LOGIC_VECTOR(13 downto 0);
signal CLKTransistor : STD_LOGIC;
signal CLKShift		: STD_LOGIC_VECTOR(1 downto 0);
signal Start			: STD_LOGIC;
signal CLKTranShift	: STD_LOGIC_VECTOR(1 downto 0);
signal ResetPWM		: STD_LOGIC;
signal DutycycleToPWM: STD_LOGIC_VECTOR(7 downto 0);

component PrescalerTransistor
    port(
	 	start : in STD_LOGIC;
		CLK 		: in STD_LOGIC;
		CLKOUT 	: out STD_LOGIC
		);
end component;

component PWM
    port(
	CLK : IN STD_LOGIC;
	Reset : IN STD_LOGIC;
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

begin
UM1 : PrescalerTransistor PORT MAP(CLK=>CLK,
						CLKOUT=>CLKTransistor,
						start =>Start );
UM2	: PWM PORT MAP(CLK=>CLK,
						DutyCycleIN=>DutycycleToPWM,
						PWMLEFTTOUT=>Right,
						PWMRIGHTOUT=>Left,
						Reset =>ResetPWM
						);
						
UM3	: Encoder PORT MAP(
						CLK=>CLK,
						HallSensor=>EncoderIn,
						EncoderOut=>TempEncouderOut,
						Reset=>Reset
						);
						
						
UM4 : seg7 PORT MAP(
						CLK=>CLK,
						number=>TempEncouderOut,
						Segm=>Segm,
						An=>An
						);
EncoderOut <= TempEncouderOut;




-------------Motor Process---------------
process(CLK,CLKShift,PastDutyCycle,DutyCycle,CLKTranShift)
variable tempstart : std_logic;
variable tempEnable : std_logic;
variable OrgiDuty		: STD_LOGIC_VECTOR(7 downto 0);
begin
	if rising_edge(CLK) then
		if ResetPWM = '0' then
			DutycycleToPWM <= DutyCycle;
		end if;
		
		if ((PastDutyCycle(7) /= DutyCycle(7)) and DutyCycle /= "00000000" )	then
			tempEnable := '0';
			tempstart := '1';
			ResetPWM <= '1';
			OrgiDuty := DutycycleToPWM;
			DutycycleToPWM <= (others => '0');
		end if;
		
		if CLKTransistor = '1' then
				tempEnable := '1';
				tempstart := '0';
				ResetPWM <= '0';
				DutycycleToPWM <= OrgiDuty;
		end if;
		
		PastDutyCycle<= DutyCycle;
		Start <= tempstart;
		Enable <= tempEnable;
	end if;

end process;
----------------End----------------------



end Behavioral;

