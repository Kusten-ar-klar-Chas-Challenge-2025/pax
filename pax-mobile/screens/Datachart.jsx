import React from "react";
import { ScrollView, Text, Image, View, StyleSheet } from "react-native";
import { useTheme } from "../theme/ThemeContext";
import { SafeAreaView, SafeAreaProvider } from "react-native-safe-area-context";
import Datacharts from "../components/DataCharts";

const Datachart = () => {
  const { theme, isDark, toggleTheme } = useTheme();
  const styles = createStyles(theme);
  return (
    <SafeAreaProvider>
      <ScrollView style={{ backgroundColor: theme.background }}>
        <SafeAreaView>
          <View style={styles.container}>
            <Datacharts />
          </View>
        </SafeAreaView>
      </ScrollView>
    </SafeAreaProvider>
  );
};

export default Datachart;

const createStyles = (theme) =>
  StyleSheet.create({
    container: {
      flex: 2,
      backgroundColor: theme.background,
      top: 0,
      padding: 1,
      alignItems: "center",
      justifyContent: "center",
    },
    card: {
      alignSelf: "center",
      justifyContent: "center",
      backgroundColor: theme.card,
      height: 300,
      width: 320,
      top: 226,
      elevation: 3,
      borderRadius: 20,
      opacity: 300,
    },
    title: {
      color: theme.textPrimary,
      fontWeight: "700",
      textAlign: "center",
      fontFamily: "Nunito",
      fontSize: 26,
      letterSpacing: 1,
    },
    smallTitle: {
      color: theme.textPrimary,
      fontWeight: "500",
      textAlign: "center",
      fontFamily: "Nunito",
      fontSize: 20,
      letterSpacing: 0,
    },
    text: {
      fontFamily: "NunitoSans",
      color: theme.textPrimary,
      fontSize: 16,
      fontWeight: "600",
      textAlign: "left",
      letterSpacing: 0.5,
      marginLeft: 40,
    },
    icon: {
      fontFamily: "NunitoSans",
      color: theme.textPrimary,
      fontSize: 26,
      fontWeight: "800",
    },
    iconDark: {
      fontFamily: "NunitoSans",
      color: theme.card,
      fontSize: 26,
      fontWeight: "800",
    },
    image: {
      width: 350,
      height: 250,
      padding: 2,
      top: 25,
    },
  });
