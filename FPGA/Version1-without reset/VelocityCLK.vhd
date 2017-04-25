
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity VelocityCLK is
port(
	CLK : in STD_LOGIC;
	VelocityCLKOut	: out STD_LOGIC 
);
end VelocityCLK;

architecture Behavioral of VelocityCLK is
begin

process(CLK)
constant Prescaler : STD_LOGIC_VECTOR (16 downto 0) :="01100001101010000";
variable Counter 	 : STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
begin
	if rising_edge(CLK) then
	VelocityCLKOut <= '0';
		if (Counter < Prescaler) then
				Counter := Counter+1;
		else 
			Counter := (others => '0');
			VelocityCLKOut <= '1';
		end if;
	end if;
end process;

end Behavioral;
