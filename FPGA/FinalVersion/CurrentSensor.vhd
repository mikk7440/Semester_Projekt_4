
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.STD_LOGIC_1164.ALL;

entity CurrentSensor is
		port
		(
			clk_50mhz	:	in 	std_logic;
	--		btn0			:	in		std_logic;
	--		led			:	OUT	std_logic_vector (7 downto 0);
			MSclk 		: 	OUT 	std_logic;
			CS				:	OUT 	std_logic;
			D0				:	in		std_logic;
			D1				:	in		std_logic;
			Current1 	:  out 	std_logic_vector(11 downto 0);
			Current2		:	out	std_logic_vector(11 downto 0)
		);
end CurrentSensor;

architecture Behavioral of CurrentSensor is
type state_type is	(
							Reset,
							Start,
							RisingEdge,
							FallingEdge
							);
signal ADC1 : std_logic_vector (11 downto 0);		--Data form ADC1 conv (12-bit)
signal ADC2 : std_logic_vector (11 downto 0);		--Data form ADC2 conv (12-bit)
signal ShiftReg0 : std_logic_vector (11 downto 0) := "000000000000";	--shift register for ADC1 spi(12-bit)
signal ShiftReg1 : std_logic_vector (11 downto 0) := "000000000000";	--shift register for ADC1 spi(12-bit)
signal state : state_type := Reset;	-- used for state mashien
	

begin

Current1<= ADC1;
Current2<= ADC2;
ADC : process (clk_50mhz)

		variable clk_scaler : integer range 0 to 4 := 0;	-- used for scaling clk_50mhz
		variable ShiftCounter : integer range 0 to 16 := 0;		-- used for counting spi clock puls
		variable delayCounter : integer := 0;
		
	begin
		if rising_edge (clk_50mhz) then
			
			clk_scaler := clk_scaler + 1;
				if clk_scaler > 4 then			-- oprindelig er "3"  - med 4 = 12,5mhz
					clk_scaler := 0;				-- reset scaler counter
					
					case state is
						
						when Reset =>
							CS   <= '1';
							MSclk <= '1';
							state <= Start;
							
						when Start =>
							
							ShiftCounter := 0;
							MSclk  <= '1';
							CS    <= '0';
							state <= RisingEdge;
							
						when RisingEdge =>
							ShiftReg0 <= ShiftReg0(10 downto 0) & D0; --shifter ADC0 bit ind
							ShiftReg1 <= ShiftReg1(10 downto 0) & D1;	--shifter ADC1 bit ind
							MSclk <= '0';
							ShiftCounter := ShiftCounter +1;
							state <= FallingEdge;
							
						when FallingEdge =>
							MSclk <= '1';
							state <= RisingEdge;
							if ShiftCounter = 16 then  -- efter 16 (12,3mhz clock) pulser
								ADC2 <= ShiftReg0;	-- gemmer endelige adc værdi
								ADC1 <= ShiftReg1;	-- gemmer endelige adc værdi
								CS <= '1';
								state <= Start;		-- starter forfra
							end if;
					end case;
				end if;
		end if;

	end process;
	
ADCTest : process(clk_50mhz)

begin
	
	if rising_edge(clk_50mhz) then
	
	--led <= "00000000";
	
	if ADC1 >= "001000000000" then -- 512
--		led <= "00000001";
	end if;
	if ADC1 >= "010000000000" then -- 512*2
--		led <= "00000011";
	end if;
	if ADC1 >= "011000000000" then -- 512*3
--		led <= "00000111";
	end if;
	if ADC1 >= "100000000000" then -- 512*4
--		led <= "00001111";
	end if;
	if ADC1 >= "101000000000" then -- 512*5
--		led <= "00011111";
	end if;
	if ADC1 >= "110000000000" then -- 512*6
--		led <= "00111111";
	end if;
	if ADC1 >= "111000000000" then -- 512*7
--		led <= "01111111";
	end if;
	if ADC1 >= "111111111111" then -- 512*8
--		led <= "11111111";
	end if;

	end if;
end process;

end Behavioral;

