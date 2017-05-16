
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity MotorController is
port( 
-------------Default---------------
		CLK  		:	IN STD_LOGIC;
		Reset		: In STD_LOGIC;

--		Led  		: 	OUT STD_LOGIC_VECTOR(7 downto 0);
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
		EncoderOut:	OUT STD_LOGIC_VECTOR(13 downto 0);
		VelocityOut  : OUT STD_LOGIC_VECTOR(11 downto 0);
--------------End------------------

		HallIndex : IN STD_LOGIC

);


end MotorController;

architecture Behavioral of MotorController is
signal PastDutyCycle	: STD_LOGIC;
signal CLKTransistor : STD_LOGIC;
signal Start			: STD_LOGIC;
signal ResetPWM		: STD_LOGIC;
signal ResetEncoder	: STD_LOGIC;
signal DutycycleToPWM: STD_LOGIC_VECTOR(7 downto 0);
signal VelocitySignal: STD_LOGIC_VECTOR(11 downto 0);
signal TempEncouderOut : STD_LOGIC_VECTOR(13 downto 0);

constant Zero8Bit : STD_LOGIC_VECTOR(7 downto 0) := (others =>'0');







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



component Velocity
port (
		CLK 			: IN STD_LOGIC;
		EncouderIN 	: IN std_logic_vector(13 downto 0);
		Velocity		: OUT std_logic_vector(11 downto 0)
		
		
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
						Reset=>ResetEncoder
						);
						
UM4	: Velocity PORT MAP(
						CLK=>CLK,
						EncouderIN=>TempEncouderOut,
						Velocity=>VelocitySignal
						);						


EncoderOut <= TempEncouderOut;
VelocityOut <= VelocitySignal;



-------------Motor Process---------------
process(CLK,PastDutyCycle,DutyCycle)
variable tempstart : std_logic;
variable tempEnable : std_logic;
variable OrgiDuty		: STD_LOGIC_VECTOR(7 downto 0);
variable Forlimit		: std_logic := '0';
variable TurnRAndL	: STD_LOGIC_VECTOR(7 downto 0):= "11010101";
variable CounterReset : STD_LOGIC_VECTOR(1 downto 0):= (others =>'0');
constant ResetDuty1 : STD_LOGIC_VECTOR(7 downto 0) := "00101101";
constant ResetDuty2 : STD_LOGIC_VECTOR(7 downto 0) := "11010101";
begin
	if rising_edge(CLK) then
--------Reset function------------------------
		if Reset = '1'  then
		ResetEncoder <= '0';
			if HallIndex = '0' then
				DutycycleToPWM <= (others => '0');
				ResetEncoder <= '1';
				CounterReset := CounterReset+1;
			else
					
				if VelocitySignal = 0 then
					if TurnRAndL = ResetDuty1 then
						TurnRAndL := ResetDuty2;
					else
						TurnRAndL := ResetDuty1;
					end if;
				end if;
				if CounterReset >= 1 and HallIndex = '1' then
					if TempEncouderOut < 2000 then
						TurnRAndL := ResetDuty1;
					else
						TurnRAndL := ResetDuty2;
					end if;
				end if;
			DutycycleToPWM <= TurnRAndL;
			end if;
			
-------------end--------------------------------------		
		 else
			CounterReset := (others => '0');
			ResetEncoder <= '0';
			DutycycleToPWM <= DutyCycle;
			tempstart := '1';
			
			if ((PastDutyCycle /= DutyCycle(7)) and DutyCycle /= "00000000" )	then
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
			
			--and Forlimit = '0'
			if Limit1 <= TempEncouderOut then   --lim1-00010011010011  lim2--00001100000111
				DutycycleToPWM <= (others => '0');
				if DutyCycle(7) = '1' then
					DutycycleToPWM <= DutyCycle;
				end if;
			elsif Limit2 >= TempEncouderOut then 
								DutycycleToPWM <= (others => '0');
				if DutyCycle(7) = '0' then
					DutycycleToPWM <= DutyCycle;
				end if;
			end if;
			
--			if (Limit1 < TempEncouderOut or Limit2 > TempEncouderOut) and Forlimit = '1'  then
--				Forlimit := '0';
--			end if;
			
			
			PastDutyCycle<= DutyCycle(7);
			Start <= tempstart;
			Enable <= tempEnable;
		end if;
	end if;

end process;
----------------End----------------------



end Behavioral;

