
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity RunningAverage is
Port(
		CLK: IN STD_LOGIC;
		Number: IN STD_LOGIC_VECTOR(8 downto 0);
		Average : out STD_LOGIC_VECTOR(8 downto 0);
		run		: IN STD_LOGIC
);
end RunningAverage;

architecture Behavioral of RunningAverage is
begin


Process(CLK)
variable Avg1			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg2			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg3			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg4			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg5			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg6			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg7			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg8			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg9			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg10			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg11			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');
variable Avg12			: STD_LOGIC_VECTOR(9 downto 0) :=(others => '0');

begin
	if rising_edge(CLK) then
		if run = '1' then



						Avg8 := Avg7;
						Avg7 := Avg6;
						Avg6 := Avg5;
						Avg5 := Avg4;
						Avg4 := Avg3;
						Avg3 := Avg2;
						Avg2 := Avg1;
						Avg1 := '0' & Number;
						
						
						
						Avg9 := (Avg1+Avg2);
						Avg9 := '0' & Avg9(9 downto 1);
						Avg10 := (Avg3+Avg4);
						Avg10 := '0' & Avg10(9 downto 1);
						Avg11 := (Avg5+ Avg6);
						Avg11 := '0' & Avg11(9 downto 1);
						Avg12 := (Avg7+ Avg8);
						Avg12 := '0' & Avg12(9 downto 1);
--						
						Avg10 := (Avg9+Avg10);
						Avg10 := '0' & Avg10(9 downto 1);
						Avg12 := (Avg11+Avg12);
						Avg12 := '0' & Avg12(9 downto 1);
						
						Avg12 := (Avg10+Avg12);
						Avg12 := '0' & Avg12(9 downto 1);
						
						--Avg8 := Avg1+Avg2+Avg3+Avg4+Avg5+Avg6+Avg7+Avg8;
						--Avg8 :="0000" & Avg8(8 downto 4);
						Average <= Avg12(8 downto 0);	
		end if;
	end if;

end process;


end Behavioral;

