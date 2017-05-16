
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity PWM is
generic(
	N 	: integer := 8
	);
	
port(
	CLK : IN STD_LOGIC;
	Reset: IN STD_LOGIC;
	DutyCycleIN : IN STD_LOGIC_VECTOR (N-1 downto 0);
	PWMRIGHTOUT	: OUT STD_LOGIC;
	PWMLEFTTOUT	: OUT STD_LOGIC
);
	
end PWM;
architecture Behavioral of PWM is
component PrescalerPWM
    port(
		CLK 		: in STD_LOGIC;
		CLKOUT 	: out STD_LOGIC
		);
end component;

--signal test_duty		: STD_LOGIC_VECTOR (7 downto 0) := "00111111";
signal PreCLK			: STD_LOGIC;
signal DutyCycle		: STD_LOGIC_VECTOR(N-1 downto 0);
begin

U1 : PrescalerPWM PORT MAP(CLK=>CLK, CLKOUT=>PreCLK);

---------------------Flip for signed values-------------------------------
	---Look for converting a signed value til usigned.
DutyCycle <= DutyCycleIN when DutyCycleIN(N-1) = '0' else
						(DutyCycleIN(N-1) & not(DutyCycleIN(N-2 downto 0))+1) when DutyCycleIN(N-1) = '1';
-------------------------------------------------------------------------
CounterP: process(CLK, PreCLK)
constant MinCounter : STD_LOGIC_VECTOR (N-2 downto 0) := (others => '0');
variable Counter 		: STD_LOGIC_VECTOR (N-2 DOWNTO 0):= (others => '1'); 
begin
	if rising_edge(CLK) then
		if (PreCLK = '1') then
			if Reset = '1' then
				Counter := (others => '1');
			else
			
				Counter := Counter-1; 
				if (Counter = DutyCycle(N-2 downto 0) and DutyCycle(N-1) ='0') then
					PWMRIGHTOUT <= '1';
				elsif (Counter = DutyCycle(N-2 downto 0) and DutyCycle(N-1) ='1') then
					PWMLEFTTOUT <= '1';
				end if;
				
				if(Counter = MinCounter) then
					Counter := (others =>'0');
					PWMRIGHTOUT <= '0';
					PWMLEFTTOUT <= '0';
				end if;
			end if;
		end if;
	end if;
end process;


end Behavioral;
