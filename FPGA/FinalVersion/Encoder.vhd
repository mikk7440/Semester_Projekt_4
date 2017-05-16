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



type states is (G1,G2,G3,G4,error);
signal HallsensorFiltered : STD_LOGIC_VECTOR(1 downto 0);
signal posision : integer range 0 to 3000 := 1000;		--posision count start value
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
			posision <= 2000;
		end if;
		
		case state is
			when G1 =>
				if (HallsensorFiltered = "10") then
					state <= G2;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "01") then
					state <= G4;
					posision <= posision - 1;
				end if;
				if HallsensorFiltered = "11" then
					state <= G3;
				end if;
	
			when G2 =>
				if (HallsensorFiltered = "11")then
					state <= G3;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "00") then
					state <= G1;
					posision <= posision - 1;
				end if;
				if HallsensorFiltered = "01" then
					state <= G4;
				end if;

			when G3 =>
				if (HallsensorFiltered = "01") then
					state <= G4;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "10") then
					state <= G2;
					posision <= posision - 1;
				end if;
				if HallsensorFiltered = "00" then
					state <= G1;
				end if;

			when G4 =>
				if (HallsensorFiltered = "00") then
					state <= G1;
					posision <= posision + 1;
				elsif (HallsensorFiltered = "11") then
					state <= G3;
					posision <= posision - 1;
				end if;
				if HallsensorFiltered = "10" then
					state <= G2;
				end if;
			when others =>
				state <= error;
		end case;
		
		
	end if;
end process;
----------------------- Motor state mashine end ------------------

EncoderOut <= STD_LOGIC_VECTOR(to_signed(posision,Bit_length));

end Behavioral;

