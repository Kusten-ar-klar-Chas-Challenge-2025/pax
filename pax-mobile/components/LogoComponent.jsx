import { StyleSheet, Text, View, Image } from 'react-native'
import React from 'react'

const LogoComponent = () => {
  return (
    <View style={styles.container}>
      <Image 
      source={require('../assets/logo/pax_logo-18.png')}
      style={styles.logo}
      resizeMode='contain'></Image>
    </View>
  )
}

export default LogoComponent

const styles = StyleSheet.create({
  container: {
    alignItems: 'center',
    marginTop: 20,
  },
  logo: {
    width: 250,
    height: 250,
  },

})