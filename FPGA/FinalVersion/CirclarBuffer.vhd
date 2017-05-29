
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity CirclarBuffer is
  generic (data_width : integer := 4;    -- width of data
           address_width : integer := 1; -- width of address
           values : integer := 2);       -- number of values
  port(
       read : in std_logic;
       write : in std_logic;
       reset : in std_logic;
       datain : in std_logic_vector(data_width-1 downto 0);
       dataout : out std_logic_vector(data_width-1 downto 0);
       data_ready : out std_logic;
       clk : in std_logic
	);
end CirclarBuffer;

architecture Behavioral of CirclarBuffer is

begin





end Behavioral;

