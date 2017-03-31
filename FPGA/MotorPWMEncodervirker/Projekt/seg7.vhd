
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
-- use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.numeric_std.ALL;
use IEEE.math_real.ALL;



entity seg7 is
	Port(
		CLK : in std_logic;
		number : in STD_LOGIC_VECTOR (13 downto 0);
		Segm : out  STD_LOGIC_VECTOR (7 downto 0);
      An :   out  STD_LOGIC_VECTOR (3 downto 0)
		);
end seg7;

architecture Behavioral of seg7 is

component OneKHz
    port(
		CLK 		: in STD_LOGIC;
		oneKHz 	: out STD_LOGIC
		);
end component;
	
	signal C0,C1,C2,C3 : integer range 0 to 255;
	signal Cout : STD_LOGIC_VECTOR(1 to 8);
	signal Clk_1KHz : STD_LOGIC;
	
	function intTo7Seg(Value: in integer) return STD_LOGIC_VECTOR is
	begin
		case Value is
			when 0 => return "11000000";
			when 1 => return "11111001";
			when 2 => return "10100100";
			when 3 => return "10110000";
			when 4 => return "10011001";
			when 5 => return "10010010";
			when 6 => return "10000010";
			when 7 => return "11111000";
			when 8 => return "10000000";
			when 9 => return "10011000";
			when 11=> return "11111111";  --Turn off display
			when others => return "11111110";
		end case;
	end intTo7Seg;
	
begin
U1 : OneKHz PORT MAP(CLK=>CLK, oneKHz=>Clk_1KHz);
	compute: process(number)
		variable C : integer range 0 to 9999 := 0;
		variable S1 : integer range 0 to 255 := 0;
		variable S2 : integer range 0 to 255 := 0;
		variable S3 : integer range 0 to 255 := 0;
		begin
		C := to_integer(unsigned(number));
		S3 := 0;
		S2 := 0;
		S1 := 0;
		
		for i in 0 to 9 loop
			if (C >= 1000) then
				C := C - 1000;
				S3 := S3 + 1;
			end if;
		end loop;
		
		for i in 0 to 9 loop
			if (C >= 100) then
				C := C - 100;
				S2 := S2 + 1;
			end if;
		end loop;
		
		for i in 0 to 9 loop
			if (C >= 10) then
				C := C - 10;
				S1 := S1 + 1;
			end if;
		end loop;
		C0 <= C;
		
		if S1 = 0 and S2 = 0 and S3 = 0 then
			C1<= 11;
		else
			C1<= S1;
		end if;

		if S2 = 0 and S3 = 0 then
			C2<= 11;
		else
			C2<= S2;
		end if;
		
		if S3 = 0 then
			C3<= 11;
		else
			C3<= S3;
		end if;


	end process;
	
	
	clock : process( Clk_1KHz )
		variable S : std_logic_vector( 1 downto 0 );
	begin
		if rising_edge( Clk_1KHz ) then
			S := S + 1;
			case S is
				when "00" => 
					Cout <= intTo7Seg(C0);
					An <= "1110";
				when "01" => 
					if C1 = 0 then
					
					end if;
					Cout <= intTo7Seg(C1);
					An <= "1101";
				when "10" => 
					Cout <= intTo7Seg(C2);
					An <= "1011";
				when "11" => 
					Cout <= intTo7Seg(C3);
					An <= "0111";
				when others => null;
			end case;
					
		end if;
	end process clock;
	
	Segm <= Cout;
	
end Behavioral;

