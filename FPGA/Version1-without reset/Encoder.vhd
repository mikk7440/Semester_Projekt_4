----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:04:23 03/24/2017 
-- Design Name: 
-- Module Name:    Encoder - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Encoder is
generic(
	Bit_length 	: integer := 14
	);

port (
		Reset : in STD_LOGIC;
		CLK : in STD_LOGIC;
		HallSensor : in STD_LOGIC_VECTOR (1 downto 0); --lilla (0) and blaa (1)
		EncoderOut : out STD_LOGIC_VECTOR(Bit_length-1 downto 0)
		);
end Encoder;

architecture Behavioral of Encoder is 


component Encoder_filter
    port(
		CLK : STD_LOGIC;
		HallSensorIn : in STD_LOGIC_VECTOR(1 downto 0);
		HallSensorOut : out STD_LOGIC_VECTOR(1 downto 0)
		);
end component;



type states is (n,nw,w,sw,s,se,e,ne, error);
signal HallsensorFiltered : STD_LOGIC_VECTOR(1 downto 0);
signal posision : integer range 0 to 3000 := 0;		--posision count start value
signal state : states;				--motor start state value  			--------Home posision---- remember to set!!!
--signal posisionV : STD_LOGIC_VECTOR(7 downto 0):=(others =>'0');
begin
U1 : Encoder_filter
PORT MAP(
						CLK=>CLK,
						HallSensorIn=>HallSensor,
						HallSensorOut=>HallsensorFiltered
						);
----------------------- Motor state mashine ------------------
process(CLK)
begin
	if rising_edge(CLK) then 		
				
		if Reset = '1' then
			posision <= 0;
			state <= n;								-----Home posision   ----- remember to set!!!!
		end if;
		
		case state is
			when n =>
				if (HallsensorFiltered = "11") then
					state <= ne;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "01") then
					state <= nw;
					posision <= posision - 1;
				end if;
	
			when nw =>
				if (HallsensorFiltered = "00")then
					state <= n;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "10") then
					state <= w;
					posision <= posision - 1;
				end if;

			when w =>
				if (HallsensorFiltered = "01") then
					state <= nw;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "00") then
					state <= sw;
					posision <= posision - 1;
				end if;

			when sw =>
				if (HallsensorFiltered = "10") then
					state <= w;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "11") then
					state <= s;
					posision <= posision - 1;
				end if;

			when s =>
				if (HallsensorFiltered = "00") then
					state <= sw;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "10") then
					state <= se;
					posision <= posision - 1;
				end if;

			when se =>
				if (HallsensorFiltered = "11") then
					state <= s;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "00") then
					state <= e;
					posision <= posision - 1;
				end if;

			when e =>
				if (HallsensorFiltered = "10") then
					state <= se;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "11") then
					state <= ne;
					posision <= posision - 1;
				end if;
				
			when ne =>
				if (HallsensorFiltered = "01") then
					state <= e;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "00") then
					state <= n;
					posision <= posision - 1;
				end if;
			when others =>
				state <= error;
		end case;
		
		
	end if;
end process;
----------------------- Motor state mashine end ------------------

EncoderOut <= STD_LOGIC_VECTOR(to_signed(posision,Bit_length));

end Behavioral;

