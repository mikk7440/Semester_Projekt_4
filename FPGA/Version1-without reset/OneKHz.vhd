
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity OneKHz is
port(
	CLK : in STD_LOGIC;
	oneKHz	: out STD_LOGIC 
);
end OneKHz;

architecture Behavioral of OneKHz is
begin

process(CLK)
constant Prescaler : STD_LOGIC_VECTOR (15 downto 0) := "1100001101010000";
variable Counter 	 : STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
begin
	if rising_edge(CLK) then
	OneKHz <= '0';
		if (Counter < Prescaler) then
				Counter := Counter+1;
		else 
			Counter := (others => '0');
			OneKHz <= '1';
		end if;
	end if;
end process;

end Behavioral;
