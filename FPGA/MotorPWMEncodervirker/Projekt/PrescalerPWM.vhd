
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity PrescalerPWM is
port(
	CLK : in STD_LOGIC;
	CLKOUT	: out STD_LOGIC 
);
end PrescalerPWM;

architecture Behavioral of PrescalerPWM is

begin
process(CLK)
variable Prescaler : STD_LOGIC_VECTOR (15 downto 0) :="0000000111110100";
variable Counter 	 : STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
begin
	if rising_edge(CLK) then
	CLKOUT <= '0';
		if (Counter < Prescaler) then
				Counter := Counter+1;
		else 
			Counter := (others => '0');
			CLKOUT <= '1';
		end if;
	end if;
end process;


end Behavioral;

