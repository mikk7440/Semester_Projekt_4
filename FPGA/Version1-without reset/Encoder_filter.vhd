
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;



entity Encoder_filter is
Port(
	CLK : STD_LOGIC;
	HallSensorIn : in STD_LOGIC_VECTOR(1 downto 0);
	HallSensorOut : out STD_LOGIC_VECTOR(1 downto 0)
);

end Encoder_filter;

architecture Behavioral of Encoder_filter is

begin
process(CLK)
begin 
	if rising_edge(CLK) then
		case HallSensorIn is 
			when "00" => HallSensorOut(1) <= '0';
			when "10" => HallSensorOut(0) <= '0';
			when "01" => HallSensorOut(0) <= '1';
			when "11" => HallSensorOut(1) <= '1';
			when others => null;
		
		end case;
	end if;
end process;

end Behavioral;

