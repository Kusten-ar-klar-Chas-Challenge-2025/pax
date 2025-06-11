import React, { useEffect, useState } from "react";
import {
  View,
  StyleSheet,
  ScrollView,
  StatusBar,
  Text,
  Dimensions,
  Pressable,
  Alert,
  Platform,
  Image,
} from "react-native";
import { LineChart, BarChart } from "react-native-chart-kit";
import { useTheme } from "../theme/ThemeContext";
import { SafeAreaView, SafeAreaProvider } from "react-native-safe-area-context";
import { MaterialCommunityIcons } from "@expo/vector-icons";

const screenWidth = Dimensions.get("window").width;

const DataCharts = () => {
  const [rooms, setRooms] = useState([]);
  const { theme, isDark, toggleTheme } = useTheme();
  const styles = createStyles(theme);
  const roomIds = [1, 2, 3, 7, 4, 5, 6];

  useEffect(() => {
    const fetchSelectedRooms = async () => {
      try {
        const requests = roomIds.map((id) =>
          fetch(`https://paxdb.vercel.app/rooms/${id}`).then((res) =>
            res.json()
          )
        );
        const results = await Promise.all(requests);
        setRooms(results);
      } catch (error) {
        console.error("Fel vid hämtning:", error);
      } finally {
        // setLoading(false);
      }
    };

    fetchSelectedRooms();
  }, []);

  const labels = rooms.map((room) => room.name);
  const temperatureData = rooms.map((room) => room.temperature || 0);
  const airQualityData = rooms.map((room) => room.air_quality || 0);
  const [tooltipPos, setTooltipPos] = useState({
    visible: false,
    x: 0,
    y: 0,
    value: 0,
    label: "",
  });

  const chartConfig = {
    backgroundGradientFrom: "#E3FAC8",
    backgroundGradientFromOpacity: 1,
    backgroundGradientTo: "#E3FAC8",
    backgroundGradientToOpacity: 1,
    decimalPlaces: 0,
    fillShadowGradientFrom: "#24524B",
    fillShadowGradientFromOpacity: 0.4,
    fillShadowGradientTo: "#24524B",
    fillShadowGradientToOpacity: 0.4,
    color: (opacity = 1) => `rgba(36, 82, 75, ${opacity})`,
    labelColor: (opacity = 1) => `rgba(36, 82, 75, ${opacity})`,
    propsForBackgroundLines: {
      stroke: "#24524B",
      strokeWidth: 0.5,
    },
    propsForDots: {
      r: "1",
      strokeWidth: "8",
      stroke: "rgb(36, 82, 75)",
    },
    propsForLabels: {
      fontWeight: 700,
      color: "rgba(36, 82, 75)",
      fontFamily: "Nunito",
      width: 150,
      height: 200,
    },
  };

  const handleDataPointClick = (data) => {
    const { index, value, x, y } = data;
    const label = labels[index];

    setTooltipPos({
      x,
      y,
      value,
      label,
      visible: true,
    });

    setTimeout(() => {
      setTooltipPos((prev) => ({ ...prev, visible: false }));
    }, 6000);
  };

  return (
    <SafeAreaProvider>
      <ScrollView style={styles.scrollView}>
        <SafeAreaView>
          <View style={styles.header}>
            <StatusBar
              barStyle={isDark ? "light-content" : "dark-content"}
              backgroundColor={theme.headerBackground}
            />
            <Pressable
              onPress={toggleTheme}
              accessible={true}
              accessibilityRole="button"
              accessibilityLabel={
                isDark ? "Byt till ljust tema" : "Byt till mörkt tema"
              }
              style={styles.toggleButton}
            >
              <MaterialCommunityIcons
                name={isDark ? "white-balance-sunny" : "weather-night"}
                size={30}
                color={theme.iconSwitchmode}
              />
            </Pressable>
            <Image
              source={require("../assets/logo/pax_logo-text-10.png")}
              style={styles.logo}
              resizeMode="contain"
            />
          </View>
          <View style={styles.container}>
            <View style={styles.chartContainer}>
              <Text style={styles.title}>Temperatur (°C)</Text>
              <LineChart
                withDots
                withShadow
                data={{
                  labels,
                  datasets: [{ data: temperatureData }],
                }}
                width={screenWidth - 30}
                height={560}
                chartConfig={chartConfig}
                bezier
                verticalLabelRotation={75}
                onDataPointClick={handleDataPointClick}
                style={{
                  borderRadius: 15,
                  height: 660,
                }}
              />
              {tooltipPos.visible && (
                <View
                  style={{
                    position: "absolute",
                    left: tooltipPos.x + 20,
                    top: tooltipPos.y - 10,
                    backgroundColor: "#24524B",
                    padding: 6,
                    borderRadius: 6,
                    zIndex: 999,
                  }}
                >
                  <Text style={{ color: "#f2ffd1", fontSize: 11 }}>
                    {tooltipPos.label}: {tooltipPos.value + "°C"}
                  </Text>
                </View>
              )}
            </View>
            <View style={styles.chartContainer}>
              <Text style={styles.title}>Luftkvalitet (%)</Text>
              <LineChart
                withDots
                withShadow
                data={{
                  labels,
                  datasets: [{ data: airQualityData }],
                }}
                width={screenWidth - 30}
                height={560}
                chartConfig={chartConfig}
                bezier
                verticalLabelRotation={75}
                onDataPointClick={handleDataPointClick}
                style={{
                  borderRadius: 15,
                  height: 660,
                }}
              />
              {tooltipPos.visible && (
                <View
                  style={{
                    position: "absolute",
                    left: tooltipPos.x + 20,
                    top: tooltipPos.y - 10,
                    backgroundColor: "#24524B",
                    padding: 6,
                    borderRadius: 6,
                    zIndex: 999,
                  }}
                >
                  <Text style={{ color: "#f2ffd1", fontSize: 11 }}>
                    {tooltipPos.label}: {tooltipPos.value + "%"}
                  </Text>
                </View>
              )}
            </View>
          </View>
        </SafeAreaView>
      </ScrollView>
    </SafeAreaProvider>
  );
};

const createStyles = (theme) =>
  StyleSheet.create({
    scrollView: {
      backgroundColor: theme.background,
      height: 1000,
    },
    header: {
      flexDirection: "row",
      justifyContent: "center",
      alignItems: "center",
      height: 50,
    },
    container: {
      flex: 1,
      justifyContent: "center",
      alignItems: "center",
      top: 60,
    },
    logo: {
      width: 150,
      height: 60,
      top: 20,
    },
    toggleButton: {
      position: "absolute",
      top: 20,
      left: 20,
      zIndex: 1,
      backgroundColor: "#E3FAC840",
      borderRadius: 25,
      padding: 4,
    },
    view: {
      backgroundColor: theme.background,
      top: 20,
    },
    chartContainer: {
      backgroundColor: theme.card,
      marginTop: 20,
      height: 500,
      width: screenWidth - 10,
      paddingTop: 15,
      borderRadius: 24,
      marginBottom: 30,
      flex: 1,
      justifyContent: "center",
      alignItems: "center",
    },
    title: {
      fontFamily: "NunitoSans",
      letterSpacing: 0.5,
      fontSize: 18,
      fontWeight: "600",
      color: theme.accent,
      marginBottom: 10,
      marginTop: 2,
    },
  });

export default DataCharts;
