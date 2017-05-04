
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;



entity PrescalerTransistor is
port(
	start : in STD_LOGIC;
	CLK : in STD_LOGIC;
	CLKOUT	: out STD_LOGIC 
);
end PrescalerTransistor;

architecture Behavioral of PrescalerTransistor is
begin
process(CLK,start)
constant Prescaler : integer :=40000;----------20; -----400 nS
variable Counter 	 : integer range 0 to 40000 := 0;
begin

	if rising_edge(CLK) then
		CLKOUT <= '0';
		if start = '1' then
			if (Counter < Prescaler) then
					Counter := Counter+1;
			else 
				Counter := 0;
				CLKOUT <= '1';
			end if;
		end if;
	end if;
end process;



end Behavioral;

